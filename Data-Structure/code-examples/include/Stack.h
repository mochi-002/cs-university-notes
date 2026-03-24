#pragma once

#include <iostream>
#include <cassert>

template <typename T>
class Stack
{
private:
    T *elements;
    int topIndex;
    int capacity;

    void resize()
    {
        capacity *= 2;
        T *newData = new T[capacity];

        for (int i = 0; i < topIndex; i++)
            newData[i] = elements[i];

        delete[] elements;
        elements = newData;
    }

public:
    Stack(int cap = 4) : topIndex(-1), capacity(cap)
    {
        elements = new T[capacity];
    }

    ~Stack()
    {
        delete[] elements;
    }

    void push(const T &val)
    {
        if (topIndex + 1 == capacity)
        {
            resize();
        }
        elements[++topIndex] = val;
    }

    void pop()
    {
        assert(!empty() && "Pop on empty stack");
        topIndex--;
    }

    T &top()
    {
        assert(!empty() && "Top on empty stack");
        return elements[topIndex];
    }

    bool empty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};