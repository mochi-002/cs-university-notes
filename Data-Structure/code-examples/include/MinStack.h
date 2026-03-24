#pragma once

#pragma once
#include "Stack.h"
#include <algorithm>

template <typename T>
class MinStack : public Stack<T>
{
private:
    Stack<T> minStack;

public:
    void push(const T &x)
    {
        Stack<T>::push(x);
        if (minStack.empty())
            minStack.push(x);
        else
            minStack.push(std::min(x, minStack.top()));
    }

    void pop()
    {
        Stack<T>::pop();
        minStack.pop();
    }

    T getMin()
    {
        return minStack.top();
    }
};