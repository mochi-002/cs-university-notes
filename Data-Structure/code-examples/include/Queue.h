#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class Queue {
private:
    T*  data;
    int frontIdx;
    int backIdx;
    int count;
    int capacity;

public:
    Queue(int cap = 8) : frontIdx(0), backIdx(-1), count(0), capacity(cap) {
        data = new T[capacity];
    }
    ~Queue() { delete[] data; }

    void enqueue(const T& val) {
        assert(count < capacity && "Queue is full");
        backIdx = (backIdx + 1) % capacity;
        data[backIdx] = val;
        count++;
    }

    T dequeue() {
        assert(!empty() && "Dequeue on empty queue");
        T val = data[frontIdx];
        frontIdx = (frontIdx + 1) % capacity;
        count--;
        return val;
    }

    T& front() const {
        assert(!empty());
        return data[frontIdx];
    }

    bool empty() const { return count == 0; }
    int  size()  const { return count; }
};
