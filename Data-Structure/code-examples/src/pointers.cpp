#include <iostream>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void dynamicAllocate(int &n)
{
    int *arr = new int[n];

    for (int i = 0; i < n; i++)
    {
        *(arr + i) = i * i;
    }

    for (int i = 0; i < n; i++)
    {
        std::cout << *(arr + i) << ' ';
    }
    std::cout << std::endl;
    
    delete[] arr;
}

