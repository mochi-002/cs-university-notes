/*
 * Rule of Three requires:
 * 1. Destructor → frees memory
 * 2. Copy constructor → deep copy
 * 3. Copy assignment operator → deep copy
 */

#include <iostream>

class DynamicArray
{
private:
    int *data;
    size_t size;

public:
    // Constructor
    DynamicArray(size_t n) : size(n)
    {
        data = new int[n]{0};
    }

    // Destructor
    ~DynamicArray()
    {
        delete[] data;
        std::cout << "\nFrom Inside: Array Removed!\n" << std::endl;
    }

    // Copy constructor
    DynamicArray(const DynamicArray &other) : size(other.size)
    {
        data = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
        std::cout << "\nFrom Inside: Done Copying Data\n" << std::endl;
    }

    // Copy assignment
    DynamicArray &operator=(const DynamicArray &other)
    {
        // self-assignments check
        if (this == &other)
            return *this;

        // free existing data
        delete[] data;

        // allocate new memory and copy
        size = other.size;
        data = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    // Access elements
    int &operator[](size_t index)
    {
        return data[index];
    }

    void print() const
    {
        for (size_t i = 0; i < size; ++i)
            std::cout << data[i] << " ";
        std::cout << std::endl;
    }

    size_t getSize() const { return size; }
};

