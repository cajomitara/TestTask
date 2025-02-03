#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

const double PI = 3.14159;

//абстрактный класс "Фигура" с методом, который будет унаследован остальными классами
class Shape {
public:
    //виртуальный метод без реализации
    virtual double getArea() const = 0;
};

//класс "Круг"
class Circle : public Shape {
private:
    //поле для хранения радиуса круга
    double radius;
public:
    //конструктор со входным параметром 
    Circle(double r) : radius(r) {};

    //функция для нахождения площади круга
    double getArea() const override {
        return PI * radius * radius;
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
        if (!isValid()) {
            throw std::invalid_argument("Данный треугольник не может существовать.");
        }
    };

    /*
        функция для проверки треугольника на существование:
        треугольник не может существовать, если хотя бы одна его сторона больше суммы двух других или равна их сумме
    */
    bool isValid() const {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }

    //функция для нахождения площади треугольника по формуле Герона
    double getArea() const override {
        double p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

};

//класс "Прямоугольник"
class Rectangle : public Shape {
private:
    //поля для хранения длин сторон прямоугольника
    double a, b;
public:
    //конструктор со входными параметрами
    Rectangle(double _a, double _b) : a(_a), b(_b) {};

    //функция для нахождения площади прямоугольника
    double getArea() const override {
        return a * b;
    }
};

int main()
{
    //тест 1 (треугольник может существовать, значения целочисленные)
    std::cout << "Test 1" << std::endl;
    Circle c1(30);
    Triangle t1(5, 4, 3);
    Rectangle r1(10, 20);

    std::cout << c1.getArea() << std::endl;
    std::cout << t1.getArea() << std::endl;
    std::cout << r1.getArea() << std::endl;

    //тест 2 (треугольник может существовать, значения дробные)
    std::cout << "Test 2" << std::endl;
    Circle c2(21.11);
    Triangle t2(10.23, 11.44, 19.10004);
    Rectangle r2(7.77, 12.121212);

    std::cout << c2.getArea() << std::endl;
    std::cout << t2.getArea() << std::endl;
    std::cout << r2.getArea() << std::endl;

    //тест 3 (треугольник не может существовать, исключение обработано)
    std::cout << "Test 3" << std::endl;
    try {
        Triangle t3(100, 200, 300);
        std::cout << t3.getArea() << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "This triangle cannot exist.";
    }
}