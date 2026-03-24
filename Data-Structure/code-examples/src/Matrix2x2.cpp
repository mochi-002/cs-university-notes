#include <iostream>
#include <string>

class Matrix2x2
{
private:
    double a11, a12, a21, a22;

public:
    Matrix2x2(double a11, double a12, double a21, double a22)
        : a11(a11), a12(a12), a21(a21), a22(a22) {}

    static Matrix2x2 identity()
    {
        return Matrix2x2(1, 0, 0, 1);
    }

    void print() const
    {
        std::cout << "[" << a11 << " " << a12 << "]\n"
                  << "[" << a21 << " " << a22 << "]\n";
    }
};