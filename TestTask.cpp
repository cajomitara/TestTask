#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>

//создание мьютекса для корректного вывода
std::mutex cout_mutex;

//абстрактный класс "Фигура" с методом, который будет унаследован остальными классами
class Shape {
public:
    //виртуальный метод без реализации
    virtual double getArea() const = 0;
    virtual std::string getName() const = 0;
};

//класс "Круг"
class Circle : public Shape {
private:
    //поле для хранения радиуса круга
    double radius;
public:
    //конструктор со входным параметром 
    Circle(double r) : radius(r) {
        if (radius < 0) {
            throw std::invalid_argument("This circle cannot exist.");
        }
    };

    //функция для нахождения площади круга
    double getArea() const override {
        return M_PI * radius * radius;
    }

    //геттер названия фигуры
    std::string getName() const override {
        return "Circle";
    }
};

//класс "Треугольник"
class Triangle : public Shape {
private:
    //поля для хранения длин сторон треугольника
    double a, b, c;
public:
    //конструктор со входными параметрами
    Triangle(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {
        if (!((a + b > c) && (a + c > b) && (b + c > a)) || a < 0 || b < 0 || c < 0) {
            throw std::invalid_argument("This triangle cannot exist.");
        }
    };

    //функция для нахождения площади треугольника по формуле Герона
    double getArea() const override {
        double p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

    //геттер названия фигуры
    std::string getName() const override {
        return "Triangle";
    }
};

//класс "Прямоугольник"
class Rectangle : public Shape {
private:
    //поля для хранения длин сторон прямоугольника
    double a, b;
public:
    //конструктор со входными параметрами
    Rectangle(double _a, double _b) : a(_a), b(_b) {
        if (a < 0 || b < 0) {
            throw std::invalid_argument("This rectangle cannot exist.");
        }
    };

    //функция для нахождения площади прямоугольника
    double getArea() const override {
        return a * b;
    }

    //геттер названия фигуры
    std::string getName() const override {
        return "Rectangle";
    }
};

//функция для вывода фигур, входящих в вектор
void printShapes(const std::vector<std::shared_ptr<Shape>> shapes) {
    for (const auto& shape : shapes) {
        std::cout << shape->getName() << " (" << shape->getArea() << ") ";
    }
    std::cout << std::endl;
}

//сортировка пузырьком
void bubbleSort(std::vector<std::shared_ptr<Shape>> shapes) {
    int n = shapes.size();

    for (int i = 0; i < n - 1; i++) {
        bool flag = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (shapes[j]->getArea() > shapes[j + 1]->getArea()) {
                std::swap(shapes[j], shapes[j + 1]);
                flag = true;
            }
        }
        if (!flag)
            break;
    }

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Sort: Bubble" << std::endl;
    std::cout << "Result: ";
    printShapes(shapes);
}

//функция для переупорядочивания вектора
int partition(std::vector<std::shared_ptr<Shape>>& shapes, int low, int high) {
    double pivot = shapes[high]->getArea();
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (shapes[j]->getArea() <= pivot) {
            i++;
            std::swap(shapes[i], shapes[j]);
        }
    }
    std::swap(shapes[i + 1], shapes[high]);
    return (i + 1);
}

//быстрая сортировка
void quickSortRecursive(std::vector<std::shared_ptr<Shape>>& shapes, int low, int high) {
    if (low < high) {
        int pi = partition(shapes, low, high);
        quickSortRecursive(shapes, low, pi - 1);
        quickSortRecursive(shapes, pi + 1, high);
    }
}

//обёртка быстрой сортировки для корректного вывода
void quickSort(std::vector<std::shared_ptr<Shape>> shapes) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    quickSortRecursive(shapes, 0, shapes.size() - 1);
    std::cout << "Sort: Quick" << std::endl;
    std::cout << "Result: ";
    printShapes(shapes);
}

int main()
{
    //объявление и инициализация вектора с фигурами
    std::vector<std::shared_ptr<Shape>> shapes = {
        std::make_shared<Circle>(5),
        std::make_shared<Triangle>(3, 4, 5),
        std::make_shared<Rectangle>(4, 6),
        std::make_shared<Circle>(3),
        std::make_shared<Triangle>(7, 10, 16),
        std::make_shared<Rectangle>(2, 8)
    };

    //вывод исходного вектора
    std::cout << "Array: ";
    printShapes(shapes);

    //копирование вектора
    std::vector<std::shared_ptr<Shape>> shapesBubble = shapes;
    std::vector<std::shared_ptr<Shape>> shapesQuick = shapes;

    //использование потоков для сортировки
    std::thread bubbleThread(bubbleSort, shapesBubble);
    std::thread quickThread(quickSort, shapesQuick);

    //блокировка потоков
    bubbleThread.join();
    quickThread.join();
}