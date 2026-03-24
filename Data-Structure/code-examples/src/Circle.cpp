#include <cmath>
#include <stdexcept>
#include <iostream>

class Circle
{
private:
    double radius;

public:
    Circle(double rad) : radius(rad) {}

    void setRadius(int rad) {
        if (rad <= 0) throw std::invalid_argument("Radius must be positive");
        radius = rad;
    }

    double calculateArea() const
    {
        return (M_PI * radius * radius);
    }

    double calculateCircumference() const
    {
        return 2 * M_PI * radius;
    }
    
    ~Circle()
    {
        std::cout << "Circle Cooked!" << std::endl;
    }
};
