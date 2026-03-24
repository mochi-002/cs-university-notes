#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class ArrayList {
private:
    T*  data;
    int sz;
    int cap;

    void resize() {
        cap *= 2;
        T* newData = new T[cap];
        for (int i = 0; i < sz; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    ArrayList(int initCap = 4) : sz(0), cap(initCap) {
        data = new T[cap];
    }

    ~ArrayList() { delete[] data; }

    // O(1) amortized
    void append(const T& val) {
        if (sz == cap) resize();
        data[sz++] = val;
    }

    // O(1) — direct indexing
    T& at(int index) {
        assert(index >= 0 && index < sz);
        return data[index];
    }

    // O(n) — shift elements left
    void deleteAt(int index) {
        assert(index >= 0 && index < sz);
        for (int i = index; i < sz - 1; i++)
            data[i] = data[i + 1];
        sz--;
    }

    // O(n) — shift elements right
    void insertAt(int index, const T& val) {
        assert(index >= 0 && index <= sz);
        if (sz == cap) resize();
        for (int i = sz; i > index; i--)
            data[i] = data[i - 1];
        data[index] = val;
        sz++;
    }

    int  length() const { return sz; }
    bool empty()  const { return sz == 0; }

    void print() const {
        cout << "[ ";
        for (int i = 0; i < sz; i++) cout << data[i] << " ";
        cout << "]\n";
    }
};

