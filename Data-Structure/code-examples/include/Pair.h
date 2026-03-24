#include <iostream>
#include <algorithm>

using namespace std;

template <typename T, typename U>
class Pair
{
public:
    T first;
    U second;
    
    Pair(const T &a, const U &b) : first(a), second(b) {}

    void swap(Pair<T, U> &other) noexcept {
        using std::swap;
        swap(first, other.first);
        swap(second, other.second);
    }

    void display() const {
        cout << "(" << first << ", " << second << ")\n";
    }

    bool operator==(const Pair<T, U> &other) const {
        return first == other.first && second == other.second;
    }
};