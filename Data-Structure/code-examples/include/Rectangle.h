#pragma once

class Rectangle
{
private:
    int width;
    int height;

public:
    Rectangle(int w, int h);
    Rectangle();
    ~Rectangle();

    int area() const;
    int perimeter() const;
    void display() const;

    void setWidth(int w);
    void setHeight(int h);
    int getWidth() const;
    int getHeight() const;

    bool operator==(const Rectangle &other) const;
    Rectangle operator+(const Rectangle &other) const;
};