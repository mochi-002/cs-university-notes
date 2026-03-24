#include "Rectangle.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Rectangle::Rectangle(int w, int h)
{
    Rectangle::setWidth(w);
    Rectangle::setHeight(h);
}

Rectangle::Rectangle() : width(1), height(1) {}

Rectangle::~Rectangle() {}

int Rectangle::area() const { return width * height; }
int Rectangle::perimeter() const { return 2 * (width + height); }
void Rectangle::display() const
{
    cout << "Rectangle(" << width << " x " << height << ")\n";
}

void Rectangle::setWidth(int w)
{
    if (w <= 0)
        throw std::invalid_argument("Width must be positive");
    width = w;
}
void Rectangle::setHeight(int h)
{
    if (h <= 0)
        throw std::invalid_argument("Height must be positive");
    height = h;
}
int Rectangle::getWidth() const { return width; }
int Rectangle::getHeight() const { return height; }

bool Rectangle::operator==(const Rectangle &other) const
{
    return width == other.width && height == other.height;
}
Rectangle Rectangle::operator+(const Rectangle &other) const
{
    int newWidth = this->getWidth() + other.getWidth();
    int newHeight = this->getHeight() + other.getHeight();
    return Rectangle(newWidth, newHeight);
}