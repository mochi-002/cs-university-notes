> **How to use this document:** Your original lecture notes appear in clearly labeled `> 📘 YOUR NOTES` blocks. All expanded content, examples, and analysis follow immediately after in clearly separated sections. Use Obsidian's collapsible headers to navigate efficiently.

---

# Table of Contents

1. [[#Lecture 1 — Classes & Pointers]]
2. [[#Lecture 2 — Stacks]]
3. [[#Lecture 3 — Queues & Templates]]
4. [[#Lecture 4 — Array Lists & Algorithm Analysis]]
5. [[#Lecture 5 — Linked Lists]]

---

# Lecture 1 — Classes & Pointers

---

## 1.1 Classes in C++

---

> 📘 **YOUR NOTES**
> 
> - A single data structure that contains data as well as functions.
> - A class defines a data type, much like a struct.
> - A class in C++ consists of its members:
>     - Data: Member variables.
>     - Member functions
> - Declaration:
> 
> ```cpp
> class Name{
>     // Code
> };
> ```
> 
> - Example — Class: Rectangle. Member Variables: Width, Height. Member Functions: Calculate Area.
> 
> ```cpp
> class rectangle {
>     int width;
>     int height;
> public:
>     int CalculateArea() {};
> };
> ```

---

### 📖 Explanation / Expansion

A **class** is the cornerstone of Object-Oriented Programming (OOP) in C++. Think of it as a blueprint that bundles **state** (data) and **behavior** (functions) together. While a `struct` in C++ also does this, the key cultural difference is: `struct` defaults to `public` access, `class` defaults to `private`.

**The Three Pillars of OOP that classes enable:**

- **Encapsulation** — Hiding internal data behind an interface.
- **Inheritance** — Building new types on top of existing ones.
- **Polymorphism** — Different types responding to the same interface differently.

**Memory Layout:** When you declare a class, no memory is allocated. Memory is only allocated when you instantiate an **object**. The compiler lays out member variables sequentially in memory (with possible padding for alignment).

```
class rectangle { int width; int height; }
sizeof(rectangle) == 8 bytes  (two 4-byte ints)
```

---

### 💻 Code Examples

**Example 1 — Basic class (simple)**

```cpp
#include <iostream>
using namespace std;

class Rectangle {
private:
    int width;
    int height;

public:
    // Constructor
    Rectangle(int w, int h) : width(w), height(h) {}

    // Member function
    int area() const {
        return width * height;
    }

    int perimeter() const {
        return 2 * (width + height);
    }
};

int main() {
    Rectangle r(5, 7);
    cout << "Area: "      << r.area()      << "\n"; // 35
    cout << "Perimeter: " << r.perimeter() << "\n"; // 24
}
```

**Example 2 — Getters, Setters, and Validation (intermediate)**

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

class Rectangle {
private:
    int width;
    int height;

public:
    Rectangle(int w, int h) {
        setWidth(w);
        setHeight(h);
    }

    // Setters with validation
    void setWidth(int w) {
        if (w <= 0) throw invalid_argument("Width must be positive");
        width = w;
    }
    void setHeight(int h) {
        if (h <= 0) throw invalid_argument("Height must be positive");
        height = h;
    }

    // Getters — const-correct: they don't modify the object
    int getWidth()  const { return width;  }
    int getHeight() const { return height; }
    int area()      const { return width * height; }

    // Operator overloading
    bool operator==(const Rectangle& other) const {
        return width == other.width && height == other.height;
    }

    void display() const {
        cout << "Rectangle(" << width << " x " << height << ")\n";
    }
};

int main() {
    Rectangle r1(5, 7);
    Rectangle r2(5, 7);
    r1.display();
    cout << "Equal: " << (r1 == r2 ? "yes" : "no") << "\n"; // yes
}
```

---

### 🌍 Real-World Application

Classes model real entities in every large software system. A `Rectangle` is trivial, but the same principle applies to:

- `std::string` — a class wrapping a character array
- `std::vector` — a class managing a dynamic array
- A `BankAccount` class that hides balance manipulation behind `deposit()`/`withdraw()` to enforce business rules

---

### 🧩 Practice Tasks

#### **Easy:** 
Create a `Circle` class with radius, area, and circumference methods. 
```cpp
class Circle
{
private:
	double radius;
	
public:
	Circle(double rad) : radius(rad) {}  
	
	void setRadius(int rad) {
		if (rad <= 0) throw std::invalid_argument("Radius must be positive");
		radius = rad;
	}
	
	double calculateArea() const
	{
		return (M_PI * radius * radius);
	}
	
	double calculateCircumference() const
	{
		return 2 * M_PI * radius;
	}
	
	~Circle()
	{
		std::cout << "Circle Cooked!" << std::endl;
	}
};
```
#### **Medium:** 
Create a `BankAccount` class. It must: prevent direct balance access, have `deposit(amount)` that rejects negatives, `withdraw(amount)` that rejects overdrafts, and a `getBalance()` getter. 
```cpp
class BankAccount
{
private:
	double balance;
	
public:
	BankAccount(double amount)
	{
		setBalance(amount);
	}
	
	void setBalance(double amount)
	{
		if (amount < 0)
			throw std::invalid_argument("Balance must be positive");
		balance = amount;
	}
	
	void deposit(double amount)
	{
		if (amount < 0)
			throw std::invalid_argument("Can not deposit with negative values");
		balance += amount;
	}
	
	void withdraw(double amount)
	{
		if (amount < 0)
			throw std::invalid_argument("Can not withdraw with negative values");
		if (amount > balance)
			throw std::invalid_argument("Can not withdraw more than you have");
		balance -= amount;
	}
	
	double getBalance() const {
		return balance;
	}
};
```
#### **Hard:** 
Overload the `+` operator for your `Rectangle` class so that `r1 + r2` returns a new rectangle with the combined area preserved (same height, summed widths).
```cpp
Rectangle Rectangle::operator+(const Rectangle &other) const
{
	int newWidth = this->getWidth() + other.getWidth();
	int newHeight = this->getHeight() + other.getHeight();
	return Rectangle(newWidth, newHeight);
}
```

---

### ⚡ Extra Insights

- **`const` after a method signature** (`int area() const`) tells the compiler this method won't modify the object. Always mark read-only methods as `const` — it enables calling them on `const` objects and is a professional best practice.
- **`struct` vs `class` in C++:** They are almost identical. `struct` members are `public` by default; `class` members are `private` by default. Use `struct` for simple data holders (POD types), `class` for types with invariants.

---

## 1.2 Member Variables & Access Modifiers

---

> 📘 **YOUR NOTES**
> 
> - **Private:** The variable can't be accessed or modified outside the class. Only member functions can change its value.
> 
> ```cpp
> private:
>     int width;
>     int height;
> ```
> 
> - **Member Functions:**
> 
> ```cpp
> public:
>     int CalculateArea() {};
> ```

---

### 📖 Explanation / Expansion

C++ has three access specifiers:

|Specifier|Accessible by|Inherited (public inheritance)|
|---|---|---|
|`public`|Everyone|Remains `public`|
|`protected`|Class + derived classes|Remains `protected`|
|`private`|Class only|Not accessible in derived class|

**The Principle of Least Privilege:** Always make members as private as possible. This protects the **class invariant** — the set of conditions that must always hold true (e.g., `width > 0`).

**Why `private` matters:** If `width` were public, any code could write `r.width = -5;`, breaking the object's logical consistency. By making it private and providing a setter that validates input, you enforce correctness.

---

### 💻 Code Examples

```cpp
class Student {
private:
    string name;
    int    grade;  // invariant: 0 <= grade <= 100

public:
    Student(const string& n, int g) : name(n) {
        setGrade(g); // use setter to enforce invariant even in constructor
    }

    void setGrade(int g) {
        if (g < 0 || g > 100)
            throw out_of_range("Grade must be 0-100");
        grade = g;
    }

    int    getGrade() const { return grade; }
    string getName()  const { return name;  }

    string letterGrade() const {
        if (grade >= 90) return "A";
        if (grade >= 80) return "B";
        if (grade >= 70) return "C";
        return "F";
    }
};
```

---

## 1.3 Class vs. Object

---

> 📘 **YOUR NOTES**
> 
> - An object is an instance of the class.
> - They share the common structure that the class defines.
> - A class is like a mold and objects are the items produced using the mold.
> 
> ```cpp
> rectangle r1; // object
> ```

---

### 📖 Explanation / Expansion

This is the **Type vs. Instance** distinction, fundamental to all OOP:

```
Class  →  Blueprint / Type definition  (zero memory for data)
Object →  Concrete instance            (holds actual data)
```

You can have thousands of `Rectangle` objects, each with different `width` and `height`, but they all share the **same code** (member functions). The functions live in the code segment of memory once; each object only stores its own data members.

**The `this` pointer:** Inside any member function, `this` is a hidden pointer to the calling object. When you write `return width * height;`, the compiler sees it as `return this->width * this->height;`.

```cpp
Rectangle r1(3, 4);  // r1.width=3, r1.height=4
Rectangle r2(10, 2); // r2.width=10, r2.height=2
// Same area() function code, but this->width differs per call
```

---

## 1.4 Constructors

---

> 📘 **YOUR NOTES**
> 
> - A constructor is a method that describes how an instance of the class is constructed.
> - Rules:
>     1. Same name as class itself.
>     2. Cannot return anything, not even void.
>     3. Called automatically with the creation of the object.
>     4. If no constructor is explicitly defined, one is automatically generated.
> - Definition (two ways):
> 
> ```cpp
> // Way 1
> rectangle(int wdth, int hgt) { width = wdth; height = hgt; }
> // Way 2 — initializer list
> rectangle(int wdth, int hgt) : width(wdth), height(hgt) {}
> ```
> 
> - Creating an object: `rectangle r1(5, 7);`
> - You can have more than one constructor.

---

### 📖 Explanation / Expansion

**Why prefer the initializer list (Way 2)?** In Way 1, member variables are _default-constructed first_, then assigned. With the initializer list, they are _directly initialized_. This matters for:

- `const` members (cannot be assigned after construction)
- Reference members
- Members of class type (avoids double initialization — more efficient)

**Types of Constructors:**

|Type|Syntax|When called|
|---|---|---|
|Default constructor|`Rectangle()`|`Rectangle r;`|
|Parameterized|`Rectangle(int, int)`|`Rectangle r(5, 7);`|
|Copy constructor|`Rectangle(const Rectangle&)`|`Rectangle r2 = r1;`|
|Move constructor (C++11)|`Rectangle(Rectangle&&)`|`Rectangle r2 = std::move(r1);`|

**Constructor Overloading — the "You can have more than one" point:**

```cpp
class Rectangle {
public:
    Rectangle() : width(0), height(0) {}           // default
    Rectangle(int w, int h) : width(w), height(h) {} // parameterized
    Rectangle(int side) : width(side), height(side) {} // square shortcut
};
```

**The Rule of Three / Five / Zero:**

- If you define a destructor, copy constructor, or copy-assignment operator, you likely need all three (Rule of Three).
- In modern C++ (C++11+), extend to Rule of Five with move constructor and move-assignment.
- Rule of Zero: prefer types that don't need manual resource management at all (use smart pointers).

---

### 💻 Code Examples

```cpp
#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    string title;
    string author;
    int    pages;

public:
    // Default constructor
    Book() : title("Unknown"), author("Unknown"), pages(0) {
        cout << "Default Book created\n";
    }

    // Parameterized constructor
    Book(const string& t, const string& a, int p)
        : title(t), author(a), pages(p) {
        cout << "Book created: " << title << "\n";
    }

    // Copy constructor
    Book(const Book& other)
        : title(other.title), author(other.author), pages(other.pages) {
        cout << "Book copied: " << title << "\n";
    }

    void display() const {
        cout << title << " by " << author << " (" << pages << " pages)\n";
    }
};

int main() {
    Book b1;                               // Default
    Book b2("Dune", "Frank Herbert", 412); // Parameterized
    Book b3 = b2;                          // Copy
    b3.display();
}
```

---

### 🧩 Practice Tasks

#### **Easy:**
Add a default constructor to your `Rectangle` that sets both dimensions to 1. 
```cpp
Rectangle::Rectangle() : width(1), height(1) {}
```
#### **Medium:**
Create a `Matrix2x2` class with 4 `double` members. Provide a parameterized constructor and an `identity()` static factory method. 
```cpp
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
```
#### **Hard:**
Implement the full Rule of Three for a `DynamicArray` class that wraps a heap-allocated `int*`.
```cpp
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
		for (size_t i = 0; i < size; ++i)\
			std::cout << data[i] << " ";
		std::cout << std::endl;
	}
	
	size_t getSize() const { return size; }
};
```
---

## 1.5 Destructors

---

> 📘 **YOUR NOTES**
> 
> - Executed when:
>     1. An object goes out of scope.
>     2. `delete` is applied to a pointer to the object.
>     3. Program ends.
>     4. Explicitly called.
> - Cannot return or take parameters.
> 
> ```cpp
> ~rectangle(void) {}
> ```

---

### 📖 Explanation / Expansion

The destructor is C++'s primary **resource cleanup** mechanism. Unlike languages with garbage collection, C++ relies on **RAII (Resource Acquisition Is Initialization)**: resources are acquired in the constructor and released in the destructor.

**When you MUST write a destructor:** When your class `new`s memory, opens a file, acquires a mutex, or manages any other resource that must be explicitly released.

```
Constructor   →  Acquire resources
(object lives)
Destructor    →  Release resources
```

**Destructor call order:** Destructors are called in **reverse order of construction**. If you create `r1`, then `r2`, then `r3`, they are destroyed as `r3`, `r2`, `r1`.

**Virtual Destructors:** If your class will be used as a base class, mark the destructor `virtual`. Otherwise, deleting a derived object through a base pointer causes undefined behavior (a classic, nasty bug).

---

### 💻 Code Examples

```cpp
#include <iostream>
using namespace std;

class ManagedArray {
private:
    int* data;
    int  size;

public:
    ManagedArray(int sz) : size(sz) {
        data = new int[size];  // Acquire heap memory
        cout << "Array of " << size << " ints allocated\n";
    }

    ~ManagedArray() {
        delete[] data;          // Release heap memory
        cout << "Array freed\n";
    }

    // RAII in action: memory is guaranteed to be freed
    // even if an exception is thrown
    void fill(int val) {
        for (int i = 0; i < size; i++) data[i] = val;
    }

    int get(int i) const { return data[i]; }
};

int main() {
    {
        ManagedArray arr(5);
        arr.fill(42);
        cout << arr.get(0) << "\n"; // 42
    } // arr goes out of scope HERE — destructor called automatically
    cout << "After scope\n";
}
```

---

### ⚡ Extra Insights

- **Modern alternative:** Use `std::unique_ptr` or `std::vector` instead of raw `new`/`delete`. They follow RAII automatically and you rarely need a manual destructor.
- **Never throw from a destructor.** If a destructor throws while another exception is active (during stack unwinding), `std::terminate()` is called.

---

## 1.6 Separation of Interface and Implementation

---

> 📘 **YOUR NOTES**
> 
> - The interface (`.h` file) lists the class and its members.
> - The implementation (`.cpp` file) provides implementations of the functions.
> - Source code that requires the interface must `#include` the interface file.

---

### 📖 Explanation / Expansion

This separation is how all professional C++ projects are structured. It provides:

1. **Compilation efficiency** — Only the `.cpp` files are recompiled when implementation changes; headers are only re-included when the interface changes.
2. **Encapsulation at the file level** — Users see only the `.h` file, not the implementation.
3. **Separate compilation** — Multiple `.cpp` files can be compiled independently and linked together.

**Header Guards / `#pragma once`:** Without them, if you `#include` a header twice, the compiler sees the class definition twice → error.

```cpp
// Old style:
#ifndef RECTANGLE_H
#define RECTANGLE_H
// ... class definition
#endif

// Modern style (non-standard but universally supported):
#pragma once
// ... class definition
```

---

### 💻 Code Examples

**`rectangle.h`** — Interface

```cpp
#pragma once

class Rectangle {
private:
    int width;
    int height;

public:
    Rectangle(int w, int h);
    Rectangle();
    ~Rectangle();

    int  area()      const;
    int  perimeter() const;
    void display()   const;

    void setWidth(int w);
    void setHeight(int h);
    int  getWidth()  const;
    int  getHeight() const;
};
```

**`rectangle.cpp`** — Implementation

```cpp
#include "rectangle.h"
#include <iostream>
using namespace std;

// Scope resolution operator :: connects each function to its class
Rectangle::Rectangle(int w, int h) : width(w), height(h) {}

Rectangle::Rectangle() : width(0), height(0) {}

Rectangle::~Rectangle() {}

int Rectangle::area()      const { return width * height; }
int Rectangle::perimeter() const { return 2 * (width + height); }

void Rectangle::display() const {
    cout << "Rectangle(" << width << " x " << height << ")\n";
}

void Rectangle::setWidth(int w)  { width = w; }
void Rectangle::setHeight(int h) { height = h; }
int  Rectangle::getWidth()  const { return width; }
int  Rectangle::getHeight() const { return height; }
```

**`main.cpp`** — Usage

```cpp
#include "rectangle.h"

int main() {
    Rectangle r(5, 7);
    r.display(); // works because main.cpp knows the interface
}
```

---

## 1.7 Scope Resolution Operator `::`

---

> 📘 **YOUR NOTES**
> 
> - Each member function must identify the class that it is part of.
> - Syntax: `ClassName::member`
> - `::` is called the scope resolution operator.
> - The signature must match exactly what's in the header.

---

### 📖 Explanation / Expansion

The `::` operator **resolves names within a scope**. It's used in three main contexts:

```cpp
Rectangle::area()       // Member of class Rectangle
std::cout               // Member of namespace std
::globalVariable        // Global scope (no qualifier = top-level)
```

**Why required for member functions outside the class?** Without `ClassName::`, the compiler doesn't know which class the function belongs to. Multiple classes could each have an `area()` function.

**Static members** also use `::`:

```cpp
class Counter {
public:
    static int count; // shared across all instances
    static void reset() { count = 0; }
};
int Counter::count = 0; // definition outside class (required for static members)
// Usage:
Counter::reset();
```

---

## 1.8 Pointers

---

> 📘 **YOUR NOTES**
> 
> - A pointer variable stores the address where another object resides.
> - Declaration:
> 
> ```cpp
> int num = 54, *numPtr;
> numPtr = &num;
> ```
> 
> - **Why Pointers?**
>     - Dynamic allocation of memory.
>     - Enable complex "linked" data structures like linked lists and trees.
> - **Pointer Assignment:**
> 
> ```cpp
> int *p1, *p2;
> p1 = p2;     // both point to same address
> *p1 = *p2;   // copy the VALUE at p2's address into p1's address
> ```
> 
> - **Initialization:** Using an uninitialized pointer causes an exception.
> - **`new` operator:** Allocates heap memory. `int *p1 = new int;`
> - **Memory Leaks:** No garbage collector. Use `delete` or memory is lost.
> - **`delete` operator:** Deallocates. Pointer still exists; location is destroyed.

---

### 📖 Explanation / Expansion

**The Memory Model — Stack vs. Heap:**

```
Stack (automatic storage)        Heap (dynamic storage)
─────────────────────            ──────────────────────
int x = 5;                       int* p = new int(5);
Freed when scope ends            YOU must call delete
Fast allocation                  Slower (OS call)
Fixed size known at compile time Size determined at runtime
```

**Pointer Arithmetic:** Pointers support arithmetic based on the size of the pointed-to type:

```cpp
int arr[5] = {10, 20, 30, 40, 50};
int* p = arr;        // points to arr[0]
p++;                 // now points to arr[1] (advances by sizeof(int) = 4 bytes)
cout << *p;          // 20
cout << *(p + 2);    // 40
```

**Common Pointer Mistakes:**

|Mistake|Consequence|
|---|---|
|Uninitialized pointer|Undefined behavior (crash or silent corruption)|
|Dangling pointer (after `delete`)|Undefined behavior|
|Memory leak (forgot `delete`)|Program consumes increasing memory|
|Double delete|Heap corruption / crash|
|`delete` on stack variable|Undefined behavior|

---

### 💻 Code Examples

**Example 1 — Pointer fundamentals**

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 42;
    int* ptr = &x;          // ptr holds the address of x

    cout << "x      = " << x      << "\n"; // 42
    cout << "&x     = " << &x     << "\n"; // some address like 0x7ffd...
    cout << "ptr    = " << ptr    << "\n"; // same address as &x
    cout << "*ptr   = " << *ptr   << "\n"; // 42 (dereference)

    *ptr = 100;             // modify x through the pointer
    cout << "x now  = " << x      << "\n"; // 100

    // Pointer to pointer
    int** pptr = &ptr;
    cout << "**pptr = " << **pptr  << "\n"; // 100
}
```

**Example 2 — Dynamic allocation and cleanup**

```cpp
#include <iostream>
using namespace std;

int main() {
    // Allocate a single int
    int* single = new int(99);
    cout << *single << "\n"; // 99
    delete single;           // MUST free
    single = nullptr;        // best practice: null after delete

    // Allocate an array
    int n = 5;
    int* arr = new int[n];
    for (int i = 0; i < n; i++) arr[i] = i * i;
    for (int i = 0; i < n; i++) cout << arr[i] << " "; // 0 1 4 9 16
    cout << "\n";
    delete[] arr;            // NOTE: delete[] for arrays
    arr = nullptr;

    // Allocate an object on heap
    // (Useful for data structures — next lectures)
    struct Node { int val; Node* next; };
    Node* node = new Node{42, nullptr};
    cout << node->val << "\n"; // 42  (arrow operator = dereference + member access)
    delete node;
}
```

**Example 3 — Pointers and classes (preview of linked structures)**

```cpp
#include <iostream>
using namespace std;

class Rectangle {
public:
    int width, height;
    Rectangle(int w, int h) : width(w), height(h) {}
    int area() const { return width * height; }
};

int main() {
    // Stack allocation
    Rectangle r1(3, 4);
    cout << r1.area() << "\n";      // 12 — dot operator

    // Heap allocation
    Rectangle* r2 = new Rectangle(5, 6);
    cout << r2->area() << "\n";     // 30 — arrow operator (equivalent to (*r2).area())
    cout << (*r2).area() << "\n";   // 30 — same thing
    delete r2;
}
```

---

### 🔗 References / What to Review

- **Prerequisites:** Stack vs. heap memory model, arrays
- **Strongly connected:** References (safer alternative to pointers), `nullptr` vs `NULL`, smart pointers (`unique_ptr`, `shared_ptr`)
- **Next lecture:** Dynamic arrays (foundation for stacks)

---

### 🌍 Real-World Application

- **Operating systems:** Memory management, process control blocks
- **Data structures:** Linked lists, trees, graphs — all use pointers to connect nodes
- **Embedded systems:** Direct hardware register access via pointers
- **Graphics engines:** Frame buffers are large arrays accessed through pointers

---

### 🧩 Practice Tasks

#### **Easy:**
Write a `swap(int*, int*)` function using pointers. 
```cpp
void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
```
#### **Medium:** 
Write a function that dynamically allocates an array of `n` integers, fills it with the first `n` squares, prints them, then frees the memory. 
```cpp
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
```
#### **Hard:**
Implement a simple `ObjectPool<T>` class that pre-allocates a fixed number of objects on the heap and dispenses/reclaims them via methods, without ever using `new`/`delete` inside the using code.
```cpp
// ToDo
Maybe in another time
```
---

### ⚡ Extra Insights

- **Prefer references over pointers** when the pointed-to thing cannot be null and won't be reassigned. References are safer (can't be null, can't do arithmetic on them).
- **`nullptr` vs `NULL` vs `0`:** Use `nullptr` in modern C++ (C++11+). It's type-safe and won't accidentally convert to integer.
- **Smart pointers:** `std::unique_ptr<T>` is the modern replacement for owning raw pointers. It calls `delete` automatically in its destructor. Use it whenever possible.

---

# Lecture 2 — Stacks

---

## 2.1 Dynamic Arrays (Prerequisite)

---

> 📘 **YOUR NOTES (from Lecture 2 slides)**
> 
> - **Problems with Static Arrays:**
>     - Exceeding maximum: Choosing a real maximum is often impossible; very large arrays waste memory.
>     - No expansion.
> - **Creating Dynamic Arrays:**
> 
> ```cpp
> int* arr = new int[5]; // pointer to first element, consecutive storage
> delete[] arr;          // cleanup
> ```

---

### 📖 Explanation / Expansion

Static arrays (`int arr[100]`) have their size fixed at **compile time**. Dynamic arrays are allocated at **runtime**, allowing the program to decide the size based on input.

**How it works in memory:**

```
Stack:    arr  →  [ address of heap block ]
Heap:             [ 0 | 0 | 0 | 0 | 0 ]   (5 ints = 20 bytes)
```

**Resizing a dynamic array** (what `std::vector` does internally):

```cpp
// When the array is full:
int* newArr = new int[newCapacity];     // 1. Allocate bigger block
for (int i = 0; i < oldSize; i++)
    newArr[i] = arr[i];                 // 2. Copy old data
delete[] arr;                           // 3. Free old block
arr = newArr;                           // 4. Update pointer
```

This is O(n) but **amortized O(1)** because the capacity is typically doubled, so resizes become increasingly rare.

---

## 2.2 The Stack ADT

---

> 📘 **YOUR NOTES**
> 
> - A stack is a list with the restriction that insertions and deletions can be performed in only one position, the end of the list (**LIFO** — Last In, First Out).
> - **Stack Operations:**
>     - `Length`: Returns the number of elements.
>     - `Push`: Adds an element to the top.
>     - `Pop`: Removes the top element.
>     - `Top`: Returns the top element.
>     - `Empty`: Returns whether the stack is empty.
> - **Implementations:** Using Linked Lists or Dynamic Arrays.

---

### 📖 Explanation / Expansion

**Abstract Data Type (ADT)** is a key concept here. An ADT defines _what_ operations exist and their logical behavior, without specifying _how_ they're implemented. The Stack ADT guarantees LIFO ordering regardless of whether you use an array or linked list under the hood.

**LIFO Intuition:** Think of a stack of plates. You can only add or remove from the top. The last plate placed is the first one removed.

```
PUSH  →  [  ]  →  [ A ]  →  [ A | B ]  →  [ A | B | C ]
                                              TOP = C
POP   →  [ A | B | C ]  →  [ A | B ]  →  [ A ]
```

**Array-based Stack Analysis:**

|Operation|Time Complexity|Notes|
|---|---|---|
|`push`|O(1) amortized|O(n) on resize|
|`pop`|O(1)||
|`top`|O(1)||
|`empty`|O(1)||
|`size`|O(1)||

---

### 💻 Code Examples

**Example 1 — Stack implemented with dynamic array (from scratch)**

```cpp
#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class Stack {
private:
    T*   elements;
    int  topIndex;    // index of top element (-1 if empty)
    int  capacity;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i <= topIndex; i++) newData[i] = elements[i];
        delete[] elements;
        elements = newData;
    }

public:
    Stack(int cap = 4) : topIndex(-1), capacity(cap) {
        elements = new T[capacity];
    }

    ~Stack() {
        delete[] elements;
    }

    void push(const T& val) {
        if (topIndex + 1 == capacity) resize();
        elements[++topIndex] = val;
    }

    void pop() {
        assert(!empty() && "Pop on empty stack");
        topIndex--;
    }

    T& top() {
        assert(!empty() && "Top on empty stack");
        return elements[topIndex];
    }

    bool empty() const { return topIndex == -1; }
    int  size()  const { return topIndex + 1;   }
};

int main() {
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    cout << s.top() << "\n";  // 30
    s.pop();
    cout << s.top() << "\n";  // 20
    cout << s.size() << "\n"; // 2
}
```

**Example 2 — Using the STL stack**

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    // Output: 3 2 1  ← LIFO confirmed
}
```

---

## 2.3 Stack STL

---

> 📘 **YOUR NOTES**
> 
> - `stack<T>` from STL.
> - Operations: `empty`, `pop`, `push`, `size`, `top`.

---

### 📖 Explanation / Expansion

`std::stack` is a **container adaptor** — it wraps another container (by default `std::deque`) and restricts its interface to stack operations. You can change the underlying container:

```cpp
#include <stack>
#include <vector>
#include <list>

stack<int>                    s1; // uses deque (default)
stack<int, vector<int>>       s2; // uses vector (more cache-friendly)
stack<int, list<int>>         s3; // uses list
```

**`vector`-based stack** is often preferred in competitive programming for cache locality and lower overhead.

---

## 2.4 Stack Applications

---

> 📘 **YOUR NOTES**
> 
> 1. **Undo operations and Backtracking** (maze solving, undo in editors)
> 2. **Balancing Symbols** — Compilers check that every `{`, `[`, `(` has a matching closing symbol. Algorithm: push opening symbols; on closing symbol, pop and check match; error if mismatch or stack non-empty at end.
> 3. **Call Stack** — Saving return addresses and parameters when functions call each other.
> 4. **Evaluating Postfix Expressions** — Push operands; on operator, pop two operands, apply, push result.
>     - Example: `6 5 2 3 + 8 * + 3 + *` → 288
> 5. **Infix to Postfix Conversion** — Using operator precedence and a stack.

---

### 📖 Explanation / Expansion

**Application 2 — Balanced Parentheses (detailed):**

```
Input: { [ ( ) ] }
Process:
  {  → push → stack: {
  [  → push → stack: { [
  (  → push → stack: { [ (
  )  → pop (  → check ) matches ( ✓ → stack: { [
  ]  → pop [  → check ] matches [ ✓ → stack: {
  }  → pop {  → check } matches { ✓ → stack: (empty)
End of input + empty stack → BALANCED ✓
```

**Application 4 — Postfix evaluation step by step:**

```
Expression: 6 5 2 3 + 8 * + 3 + *

Token  Action          Stack
6      push 6          [6]
5      push 5          [6, 5]
2      push 2          [6, 5, 2]
3      push 3          [6, 5, 2, 3]
+      pop 3,2 → 5     [6, 5, 5]
8      push 8          [6, 5, 5, 8]
*      pop 8,5 → 40    [6, 5, 40]
+      pop 40,5 → 45   [6, 45]
3      push 3          [6, 45, 3]
+      pop 3,45 → 48   [6, 48]
*      pop 48,6 → 288  [288]

Result: 288 ✓
```

---

### 💻 Code Examples

**Balanced Parentheses Checker**

```cpp
#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool isBalanced(const string& expr) {
    stack<char> st;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char top = st.top();
            st.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{'))
                return false;
        }
    }
    return st.empty();
}

int main() {
    cout << isBalanced("{[()]}")  << "\n"; // 1 (true)
    cout << isBalanced("{[(])")   << "\n"; // 0 (false)
    cout << isBalanced("((())")  << "\n"; // 0 (false — unmatched)
}
```

**Postfix Evaluator**

```cpp
#include <iostream>
#include <stack>
#include <sstream>
#include <string>
using namespace std;

int evaluatePostfix(const string& expr) {
    stack<int> st;
    istringstream iss(expr);
    string token;
    while (iss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            if      (token == "+") st.push(a + b);
            else if (token == "-") st.push(a - b);
            else if (token == "*") st.push(a * b);
            else if (token == "/") st.push(a / b);
        } else {
            st.push(stoi(token));
        }
    }
    return st.top();
}

int main() {
    // 6 5 2 3 + 8 * + 3 + * == 288
    cout << evaluatePostfix("6 5 2 3 + 8 * + 3 + *") << "\n"; // 288
}
```

---

### 🔗 References / What to Review

- **Related topics:** Recursion (call stack), expression parsing, DFS graph traversal (uses stack)
- **Competitive programming:** Monotonic stack for "next greater element" problems

---

### 🌍 Real-World Application

- **IDEs and editors:** Undo/redo history
- **Compilers:** Syntax checking, expression evaluation, function call frames
- **Browsers:** Back/forward navigation history
- **Runtime:** Every function call in any program uses the call stack

---

### 🧩 Practice Tasks

#### **Easy:** 
Using `std::stack`, reverse a string.
```cpp
string reverse_string(string s)
{
	stack<char> stk;
	for (auto ch : s)
		stk.push(ch);
	string ans = "";
	while (!stk.empty())
	{
		ans.push_back(stk.top());
		stk.pop();
	}
	return ans;
}
```
#### **Medium (from lecture):** 
String editing with `#` (backspace) and `$` (repeat last char).

```
Input: "ac$db#"
Steps: a→[a], c→[a,c], $→[a,c,c], d→[a,c,c,d], b→[a,c,c,d,b], #→[a,c,c,d]
Output: "accd"
```

```cpp
string slide34Lec2(string s) {
	stack<char> stk;
	string ans = "";
	for (auto ch : s) 
	{
		if (ch == '#') 
		{
			stk.pop();
		} 
		else if (ch == '$') 
		{
		stk.push(stk.top());
		} 
		else 
		{
		stk.push(ch);
		}
	}
	while (!stk.empty()) 
	{
		ans.push_back(stk.top());
		stk.pop();
	}
	string realAns = "";
	for (int i = ans.size() - 1; i >= 0; i--) 
	{
		realAns.push_back(ans[i]);
	}
	return realAns;
}
```
#### **Hard (from lecture):**
Card game points:
- `W`: +5 points
- `D`: +10 points
- `C`: cancel last round
- `S`: sum of last 2 valid rounds Given a sequence, return total points.

```txt
مكسل اعمله
```

#### **Competitive:** 
Implement a stack that supports `push(x)`, `pop()`, `top()`, and `getMin()` — all in O(1) time. (Hint: use an auxiliary min-stack.)
```cpp
class MinStack {
private:
    stack<int> mainStack;
    stack<int> minStack;

public:
    void push(int x) {
        mainStack.push(x);
        
        if (minStack.empty()) {
            minStack.push(x);
        } else {
            minStack.push(min(x, minStack.top()));
        }
    }

    void pop() {
        if (!mainStack.empty()) {
            mainStack.pop();
            minStack.pop();
        }
    }

    int top() {
        return mainStack.top();
    }

    int getMin() {
        return minStack.top();
    }
};
```
---

### ⚡ Extra Insights

- **Stack overflow:** The program's call stack has limited size. Infinite recursion or very deep recursion causes a stack overflow crash. Iterative solutions using explicit stacks avoid this.
- **Monotonic stack** is a competitive programming pattern where you maintain a stack in sorted order to answer "next greater element" queries in O(n).

---

# Lecture 3 — Queues & Templates

---

## 3.1 Templates

---

> 📘 **YOUR NOTES (from Lecture 3 slides)**
> 
> - Some algorithms are type-independent.
> - It's preferable to write code once rather than recode for each type.
> - Achieved using Templates.
> 
> ```cpp
> template <class N>
> class rectangle {
> private:
>     N width;
>     N height;
> public:
>     rectangle(N, N);
>     N area();
> };
> 
> // Member function:
> template <class N>
> N rectangle<N>::area() { return width * height; }
> 
> // Declaring objects:
> rectangle<float> r1(5.3, 7.0);
> rectangle<int>   r2(5, 7);
> ```

---

### 📖 Explanation / Expansion

**Templates** are C++'s mechanism for **generic programming** — writing code that works for any type while remaining type-safe. The compiler **instantiates** (generates concrete code for) a template at compile time for each type you use.

```
template<class T>
void swap(T& a, T& b)  →  compiler generates:
    void swap(int& a, int& b)     (when called with ints)
    void swap(string& a, string& b) (when called with strings)
```

**Two kinds of templates:**

1. **Function templates** — generic functions
2. **Class templates** — generic classes (what your notes show)

**`typename` vs `class` keyword:** Inside `template<>`, both `typename` and `class` mean the same thing. `typename` is preferred in modern C++ for clarity.

---

### 💻 Code Examples

**Function template**

```cpp
#include <iostream>
using namespace std;

template <typename T>
T maxOf(T a, T b) {
    return (a > b) ? a : b;
}

// Specialization for char* (strings need strcmp, not >)
template <>
const char* maxOf(const char* a, const char* b) {
    return (strcmp(a, b) > 0) ? a : b;
}

int main() {
    cout << maxOf(3, 7)         << "\n"; // 7
    cout << maxOf(3.14, 2.71)   << "\n"; // 3.14
    cout << maxOf('a', 'z')     << "\n"; // z
}
```

**Class template — generic Stack**

```cpp
template <typename T>
class Stack {
private:
    T*  data;
    int top;
    int capacity;
public:
    Stack(int cap = 16) : top(-1), capacity(cap) {
        data = new T[capacity];
    }
    ~Stack() { delete[] data; }
    void push(const T& val) { data[++top] = val; }
    void pop()              { --top; }
    T&   peek()             { return data[top]; }
    bool empty() const      { return top == -1; }
};

// Works for any type:
Stack<int>    intStack;
Stack<string> strStack;
Stack<double> dblStack;
```

---

### 🧩 Practice Tasks

#### **Medium:** 
Write a generic `Pair<T, U>` class template with `first` and `second` members, a constructor, and a `swap()` method.
```cpp
template <typename T, typename U>
class Pair
{
public:
	T first;
	U second;
	
	Pair(const T &a, const U &b) : first(a), second(b) {}
	
	void swap(Pair<T, U> &other) {
		using std::swap;
		swap(first, other.first);
		swap(second, other.second);
	}
	
	void display() const {
		cout << "(" << first << ", " << second << ")\n";
	}
};
```
#### **Hard:** 
Write a generic `MinStack<T>` that tracks minimum in O(1). The type `T` must support `<`.
```cpp
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
```
---

## 3.2 The Queue ADT

---

> 📘 **YOUR NOTES**
> 
> - Like stacks, queues are lists. Insertion at one end, deletion at the other (**FIFO** — First In, First Out).
> - **Applications:** Job scheduling (printers), call center queues, queuing theory.
> - **Operations:**
>     - `Length`: Returns number of elements.
>     - `Enqueue`: Adds an element to the end.
>     - `Dequeue`: Removes the front element.
>     - `Front`: Returns the front element.
>     - `Empty`: Returns whether the queue is empty.
> - **Implementations:** Linked Lists or Dynamic Arrays.

---

### 📖 Explanation / Expansion

**FIFO Intuition:** A physical queue at a bank. First person to arrive is first person served. Fair ordering.

```
ENQUEUE A → [A]
ENQUEUE B → [A | B]
ENQUEUE C → [A | B | C]
DEQUEUE   → returns A → [B | C]
DEQUEUE   → returns B → [C]
```

**The Circular Array Problem:** Your notes mention this. A naive array queue wastes space as front advances:

```
After 3 enqueues and 2 dequeues:
[  |  | C ]   ← front=2, wasted space at indices 0,1
```

If we just keep enqueuing, we run out of space even though there's empty space at the front!

**Circular Array Solution:** Treat the array as circular using modulo arithmetic:

```cpp
back = (back + 1) % capacity;   // wrap around
```

This reuses the freed front space.

---

### 💻 Code Examples

**Circular Array Queue — from scratch**

```cpp
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

int main() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    cout << q.dequeue() << "\n"; // 1 (FIFO)
    cout << q.dequeue() << "\n"; // 2
    q.enqueue(4);
    cout << q.front()   << "\n"; // 3
}
```

**STL Queue**

```cpp
#include <queue>
#include <iostream>
using namespace std;

int main() {
    queue<int> q;
    q.push(1);   // enqueue
    q.push(2);
    q.push(3);

    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();           // dequeue
    }
    // Output: 1 2 3  ← FIFO confirmed
}
```

---

## 3.3 Queue STL & Deque

---

> 📘 **YOUR NOTES**
> 
> - `queue<T>` — standard FIFO queue.
>     - Operations: `empty`, `pop` (= dequeue), `push` (= enqueue), `size`, `front`, `back`
> - `deque<T>` — double-ended queue.
>     - Operations: `push_back`, `push_front`, `pop_back`, `pop_front`, `front`, `back`, `size`, `empty`

---

### 📖 Explanation / Expansion

`std::deque` (pronounced "deck") is a powerful data structure that allows O(1) insertion and deletion at **both** ends. It's the default underlying container for `std::queue` and `std::stack`.

```
push_front → [ A | B | C | D ] ← push_back
pop_front  ←                   → pop_back
```

**When to use `deque` over `vector`:**

- When you need frequent insertions/deletions at the front
- When you don't need contiguous memory (deque allocates in chunks)
- Slightly slower than `vector` for random access due to pointer indirection

**`priority_queue`** (preview — covered later): A queue where the highest-priority element is always at the front.

---

### 🌍 Real-World Application

- **OS scheduling:** CPU ready queue, I/O request queues
- **BFS (Breadth-First Search):** Graph traversal uses a queue — covered in the Graphs lecture
- **Network routers:** Packet queues
- **Message queues:** Kafka, RabbitMQ — industrial-scale FIFO message passing

---

### 🧩 Practice Tasks

#### **Easy (from lecture):** 
Card deck problem: Given `n` cards (1 to n, card 1 on top), repeatedly discard the top card and move the new top to the bottom. Find the order of discarded cards and the last remaining card.

```
n=10: Discarded: 1, 3, 5, 7, 9, 2, 6, 10, 8 | Remaining: 4
```

```cpp
void cardDeck(int n) {
	queue<int> deck;
	for (int i = 1; i <= n; i++)
		deck.push(i);
	  
	cout << "Discarded: ";
	while (deck.size() > 1) {
		cout << deck.front() << " ";
		deck.pop();
		deck.push(deck.front());
		deck.pop();
	}
	cout << "| Remaining: " << deck.front() << endl;
}
```
#### **Medium (from lecture):** 
Write `replace(Q, v1, v2)` that replaces all occurrences of `v1` with `v2` in a circular queue using only queue operations.

```cpp
template <typename T> 
void replace(queue<T> &Q, T v1, T v2) { 
	int size = Q.size(); 
	for (int i = 0; i < size; i++) { 
		T val = Q.front(); 
		Q.pop(); 
		Q.push(val == v1 ? v2 : val);
	}
}
```
#### **Hard:** 
Implement a queue using two stacks. Both `enqueue` and `dequeue` should work correctly (amortized O(1)).
```cpp
template <typename T>
class MyQueue {
private:
    stack<T> inbox;    // for enqueue
    stack<T> outbox;   // for dequeue

    void transfer() {
        if (outbox.empty())
            while (!inbox.empty()) {
                outbox.push(inbox.top());
                inbox.pop();
            }
    }

public:
    void enqueue(T val) {
        inbox.push(val);
    }

    void dequeue() {
        transfer();
        if (!outbox.empty())
            outbox.pop();
    }

    T front() {
        transfer();
        return outbox.top();
    }

    bool empty() {
        return inbox.empty() && outbox.empty();
    }

    int size() {
        return inbox.size() + outbox.size();
    }
};
```
---

### ⚡ Extra Insights

- **Queue from two stacks trick:** This is a classic interview question. Use stack1 for enqueuing and stack2 for dequeuing. When stack2 is empty, pour all of stack1 into stack2 (reversing order = queue order).
- **Circular buffer** is the most memory-efficient queue implementation and is widely used in embedded systems and OS kernels for ring buffers.

---

# Lecture 4 — Array Lists & Algorithm Analysis

---

## 4.1 The List ADT

---

> 📘 **YOUR NOTES (from Lecture 4)**
> 
> - The list is of the form A₀, A₁, A₂, . . ., Aₙ₋₁.
> - A List is a finite, ordered sequence of data items.
> - Operations: `Length`, `Find`, `Delete`, `Insert`, `Append`.
> - Implemented using Linked Lists or Array Lists.

---

### 📖 Explanation / Expansion

The **List ADT** is a fundamental abstraction. It models an ordered collection where position matters. Unlike a Set (unordered, no duplicates), a List:

- Preserves insertion order
- Allows duplicate values
- Supports positional access (element at index i)

**List vs. Array:** An array is a fixed-size, contiguous memory block. A List is an abstract concept that can be implemented many ways — the two primary implementations are **Array Lists** (this lecture) and **Linked Lists** (next lecture).

---

## 4.2 Array Lists

---

> 📘 **YOUR NOTES**
> 
> - An implementation of the List ADT using dynamic arrays.
> - Can grow and shrink in size.
> - Inserting and deleting may require shifting elements.
> - **Operations:**
>     - `Length`: O(1)
>     - `Append`: O(1) amortized
>     - `At`: O(1)
>     - `DeleteAt`: O(n) — need to shift elements
>     - `InsertAt`: O(n) — need to shift elements

---

### 📖 Explanation / Expansion

**Why shifting is required:**

```
Array:   [ A | B | C | D | E ]
DeleteAt(1) removes B:
Step 1:  [ A | C | C | D | E ]   (copy C left)
Step 2:  [ A | C | D | D | E ]   (copy D left)
Step 3:  [ A | C | D | E | E ]   (copy E left)
Result:  [ A | C | D | E ]       (decrement size)
```

In the worst case (delete at index 0), all n-1 elements shift → O(n).

**Capacity vs. Size:**

- **`size`** — number of elements currently stored
- **`capacity`** — total slots allocated

When `size == capacity`, a resize is triggered. Capacity is typically doubled (geometric growth), keeping amortized append cost at O(1).

```
size=5, capacity=8:  [ 1 | 2 | 3 | 4 | 5 | _ | _ | _ ]
```

---

### 💻 Code Examples

**ArrayList from scratch**

```cpp
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

int main() {
    ArrayList<int> list;
    list.append(10);
    list.append(20);
    list.append(30);
    list.print();         // [ 10 20 30 ]

    list.insertAt(1, 15);
    list.print();         // [ 10 15 20 30 ]

    list.deleteAt(2);
    list.print();         // [ 10 15 30 ]

    cout << list.at(1) << "\n"; // 15
}
```

---

## 4.3 Vectors (STL ArrayList)

---

> 📘 **YOUR NOTES**
> 
> - Vectors are sequence containers representing arrays that can change in size.
> - Use contiguous storage.
> - Internally use a dynamically allocated array.
> - Key functions: `at`, `capacity`, `clear`, `data`, `empty`, `push_back`, `pop_back`, `size`.

---

### 📖 Explanation / Expansion

`std::vector<T>` is the STL's ArrayList implementation. It is the **most commonly used container** in C++. Unless you have a specific reason not to, start with `vector`.

**Important additional functions:**

```cpp
v.reserve(n);         // pre-allocate capacity for n elements (no resize cost)
v.shrink_to_fit();    // release excess capacity
v.insert(it, val);    // insert at iterator position — O(n)
v.erase(it);          // remove at iterator position — O(n)
v.front();            // first element
v.back();             // last element
v.begin() / v.end();  // iterators
```

**Range-based for loop (modern C++):**

```cpp
vector<int> v = {1, 2, 3, 4, 5};
for (int x : v) cout << x << " ";          // by value
for (const int& x : v) cout << x << " ";   // by const reference (preferred)
for (int& x : v) x *= 2;                   // by reference (modify elements)
```

---

### 💻 Code Examples

```cpp
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v;
    v.reserve(10); // avoid reallocations

    for (int i = 1; i <= 5; i++) v.push_back(i * 10);
    // v = [10, 20, 30, 40, 50]

    cout << "Size:     " << v.size()     << "\n"; // 5
    cout << "Capacity: " << v.capacity() << "\n"; // 10 (reserved)
    cout << "At[2]:    " << v.at(2)      << "\n"; // 30

    v.insert(v.begin() + 2, 25);  // [10, 20, 25, 30, 40, 50]
    v.erase(v.begin() + 4);       // [10, 20, 25, 30, 50]

    // Sort
    sort(v.begin(), v.end());
    for (int x : v) cout << x << " ";
    cout << "\n";

    // Binary search (requires sorted vector)
    bool found = binary_search(v.begin(), v.end(), 25);
    cout << "Found 25: " << found << "\n"; // 1
}
```

---

## 4.4 Iterators

---

> 📘 **YOUR NOTES**
> 
> - An Iterator is an object that can traverse a container without knowing its implementation.
> - Best visualized as a pointer to a given element.
> 
> ```cpp
> vector<int>::iterator it;
> it = V.begin();
> while (it != V.end()) {
>     cout << *it << endl;
>     it++;
> }
> ```

---

### 📖 Explanation / Expansion

Iterators are a **design pattern** (the Iterator pattern) that decouples traversal logic from container implementation. The STL uses them extensively to make algorithms work on any container.

**Iterator categories (from least to most powerful):**

1. **Input/Output** — single-pass read/write
2. **Forward** — multi-pass, one direction (e.g., `std::forward_list`)
3. **Bidirectional** — forward and backward (e.g., `std::list`)
4. **Random Access** — jump to any position in O(1) (e.g., `std::vector`, `std::deque`)

**`auto` keyword simplifies iterator usage (modern C++):**

```cpp
auto it = v.begin();   // instead of vector<int>::iterator it = v.begin()
```

**STL algorithms with iterators:**

```cpp
sort(v.begin(), v.end());
auto it = find(v.begin(), v.end(), 30);  // returns iterator to element
int count = count_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
```

---

## 4.5 Algorithm Analysis & Big-O Notation

---

> 📘 **YOUR NOTES**
> 
> - An algorithm is a clearly specified set of simple instructions to solve a problem.
> - We measure efficiency in terms of **time** and **storage**.
> - Running time depends on: **size of input** and **structure**.
> - **Primitive operations:** assignment, array indexing, arithmetic, field access, return.
> - **Worst-case complexity:** maximum operations performed on any input of size n.
> - **Big-O Notation:** asymptotic analysis — look at growth as n grows, drop constants and lower-order terms.
>     - e.g., `5n² + 3n + 2 → O(n²)`
> - **Order categories:** O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(aⁿ)
> - **Array List operation complexities:**
>     - `Append`: O(n) [expansion possible]
>     - `InsertAt`: O(n) [shifting]
>     - `DeleteAt`: O(n) [shifting]
>     - `At`: O(1) [direct index]

---

### 📖 Explanation / Expansion

**Formal Big-O Definition:** `f(n) = O(g(n))` means there exist constants `c > 0` and `n₀` such that `f(n) ≤ c·g(n)` for all `n ≥ n₀`.

In practice: identify the dominant term in your operation count, drop constants.

**Three cases:**

- **Best case (Ω - Omega):** Fastest possible scenario
- **Average case (Θ - Theta):** Expected scenario
- **Worst case (O - Big-O):** Slowest possible scenario

We almost always care about **worst case** in algorithm analysis.

**Complexity Hierarchy — what it means practically:**

|Complexity|n = 1,000|n = 1,000,000|Real-world implication|
|---|---|---|---|
|O(1)|1 op|1 op|Always instant|
|O(log n)|~10 ops|~20 ops|Binary search|
|O(n)|1,000|1,000,000|Linear scan|
|O(n log n)|~10,000|~20M|Merge sort|
|O(n²)|1M|10¹²|Bubble sort — unusable for large n|
|O(2ⁿ)|10³⁰⁰|—|Infeasible beyond n~50|

**The counting example from your notes:**
```cpp
findMax (int[] a, int n) {
	currentMax = a[0];
	for (int i = 0; i < n; i++) {
		if (a[i] > currentMax) {
			currentMax = a[i]; 
		}
	}
	return currentMax; 
}
```
```
findMax (int[] a, int n):
  Line 2: 2 operations (index + assign)
  Line 3: 3n+1 operations (init + n comparisons + 2n increments)
  Line 4: 2n operations (n index + n compare)
  Line 5: 2n operations (worst case: always update max)
  Line 6: 1 operation (return)
  Total: 7n + 4 → O(n)
```

---

### 💻 Code Examples

**Binary Search — O(log n)**

```cpp
#include <iostream>
#include <vector>
using namespace std;

// Returns index of target, or -1 if not found
int binarySearch(const vector<int>& arr, int target) {
    int lo = 0, hi = (int)arr.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2; // avoids overflow vs (lo+hi)/2
        if      (arr[mid] == target) return mid;
        else if (arr[mid]  < target) lo = mid + 1;
        else                         hi = mid - 1;
    }
    return -1;
}

int main() {
    vector<int> v = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    cout << binarySearch(v, 7)  << "\n"; // 3
    cout << binarySearch(v, 6)  << "\n"; // -1
}
```

**Visualizing O(n²) vs O(n log n):**

```cpp
// O(n²) — Bubble Sort
void bubbleSort(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - 1; j++)
            if (v[j] > v[j+1])
                swap(v[j], v[j+1]);
}

// O(n log n) — std::sort (introsort: quicksort + heapsort + insertion sort)
// Just use: sort(v.begin(), v.end());
```

---

### 🔗 References / What to Review

- **Prerequisites:** Basic math — logarithms, exponents, summation
- **Strongly connected:** Recurrence relations, Master Theorem (for recursive algorithms), amortized analysis
- **Topics to revise:** Sequential vs. binary search, sorting algorithms and their complexities

---

### 🌍 Real-World Application

- **Database indexing:** B-trees give O(log n) lookup vs O(n) sequential scan — the difference between a fast query and a slow one
- **System design interviews:** Knowing complexities of operations is mandatory
- **Competitive programming:** Every solution must be checked for whether it fits within the time limit given n constraints

---

### 🧩 Practice Tasks

#### **Easy:** 
Determine the Big-O of the following:

```
for i = 1 to n:
  for j = i to n:
    print(i, j)
```
```
anwer -> O(n²)
```
#### **Medium:**
Write a function that finds the two elements in an array that sum to a target value. First do it in O(n²), then optimize to O(n) using a hash set.
```cpp
// O(n²)
void findTwoSum(vector<int> &nums, int target)
{
	int n = nums.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (nums[i] + nums[j] == target)
			{
				cout << "Pair is: (" << nums[i] << ", " << nums[j] << ")\n";
				return;
			}
		}
	}
	cout << "No Valid Pair\n";
}
```
```cpp
// O(n)
void findTwoSum(vector<int> &nums, int target)
{
	unordered_set<int> seen;
	for (int num : nums)
	{
		int complement = target - num;
		if (seen.find(complement) != seen.end())
		{
			cout << "Pair is: (" << num << ", " << complement << ")\n";
			return;
		}
		cout << "No Valid Pair\n";
	}
}
```
#### **Hard:** 
Given a sorted array rotated at some pivot (e.g., `[4,5,6,7,0,1,2]`), find the index of a target element in O(log n).
```cpp
int searchRotatedArray(vector<int> &nums, int target)
{
	int left = 0;
	int right = nums.size() - 1;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (nums[mid] == target)
		{
			return mid;
		}
		if (nums[left] <= nums[mid])
		{
			if (target >= nums[left] && target < nums[mid])
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}
		else
		{
			if (target > nums[mid] && target <= nums[right])
			{
				left = mid + 1;
			}
			else
			{
				right = mid - 1;
			}
		}
	}
	return -1;
}
```
---

### ⚡ Extra Insights

- **Amortized O(1) for append:** Individual appends are O(1) most of the time but O(n) on resize. The total cost for n appends is O(n), so amortized per-operation cost is O(1).
- **Space complexity:** Don't forget to analyze memory usage too. O(n) space for an auxiliary array might be unacceptable in memory-constrained environments.
- **Competitive programming:** Always calculate if your algorithm fits: if n ≤ 10⁵ and time limit is 1s, O(n²) = 10¹⁰ operations → TLE. O(n log n) = ~1.7×10⁶ → fine.

---

# Lecture 5 — Linked Lists

---

## 5.1 Linked Lists

---

> 📘 **YOUR NOTES (from Lecture 5)**
> 
> - **Structure:**
> 
> ```
> Head → [A0|→] → [A1|→] → [A2|→] → [A3|→] → NULL
>                                              ↑ Tail
> ```
> 
> - **Operations:** Length, Append, At, InsertAt, DeleteAt.
> - **InsertAt(2):** Traverse to node before position, redirect pointers.
> - **DeleteAt(A2):** Find node before A2, redirect its next pointer to skip A2, free A2.
> - **When to use Linked List:**
>     - You don't know how many items will be in the list.
>     - You don't need random access to elements.
> - **When to use Array List:**
>     - You need indexed/random access.
>     - You know the number of elements ahead of time.
>     - Memory is a concern.

---

### 📖 Explanation / Expansion

**Memory Layout Comparison:**

```
Array List (contiguous memory):
[10][20][30][40][50]
 ↑
index 0 is at a fixed address; index k is at base + k*sizeof(T)
→ O(1) random access

Linked List (scattered nodes):
[10|addr2] → [20|addr3] → [30|addr4] → [40|null]
  addr1         addr2         addr3       addr4
→ Must traverse from head to reach element at position k → O(k)
```

**Linked List Node Structure:**

```cpp
struct Node {
    int   data;
    Node* next;

    Node(int d) : data(d), next(nullptr) {}
};
```

**The Insert and Delete pointer dance:**

```
// InsertAt(2, Ax):
// Before: A0 → A1 → A2 → A3 → NULL
// Steps:
//   1. Traverse to node at index 1 (A1)
//   2. newNode->next = A1->next  (Ax now points to A2)
//   3. A1->next = newNode        (A1 now points to Ax)
// After:  A0 → A1 → Ax → A2 → A3 → NULL

// DeleteAt(2, A2):
// Before: A0 → A1 → A2 → A3 → NULL
// Steps:
//   1. Traverse to node at index 1 (A1)
//   2. temp = A1->next           (save A2 pointer)
//   3. A1->next = A2->next       (A1 now points to A3)
//   4. delete temp               (free A2)
// After:  A0 → A1 → A3 → NULL
```

**Complexity Comparison — Array List vs. Linked List:**

|Operation|Array List|Linked List|
|---|---|---|
|`at(i)`|O(1) ✅|O(n) ❌|
|`append`|O(1) amort|O(1) with tail ptr ✅|
|`insertAt(0)`|O(n) ❌|O(1) ✅|
|`insertAt(i)`|O(n) ❌|O(n) ❌ (traverse)|
|`deleteAt(0)`|O(n) ❌|O(1) ✅|
|Memory|Contiguous (cache-friendly) ✅|Scattered (cache-hostile) ❌|
|Memory overhead|Low ✅|Extra pointer per node ❌|

---

### 💻 Code Examples

**Singly Linked List — complete implementation**

```cpp
#include <iostream>
#include <cassert>
using namespace std;

struct Node {
    int   data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    Node* tail;
    int   sz;

public:
    LinkedList() : head(nullptr), tail(nullptr), sz(0) {}

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    // O(1) — we maintain a tail pointer
    void append(int val) {
        Node* newNode = new Node(val);
        if (!tail) {
			head = tail = newNode; 
		}
        else { 
	        tail->next = newNode; tail = newNode; 
		}
        sz++;
    }

    // O(n) — traverse to position
    int at(int index) const {
        assert(index >= 0 && index < sz);
        Node* curr = head;
        for (int i = 0; i < index; i++) 
	        curr = curr->next;
        return curr->data;
    }

    // O(n) — traverse to position before insert point
    void insertAt(int index, int val) {
        assert(index >= 0 && index <= sz);
        Node* newNode = new Node(val);
        if (index == 0) {
            newNode->next = head;
            head = newNode;
            if (!tail) 
	            tail = newNode;
        } else {
            Node* prev = head;
            for (int i = 0; i < index - 1; i++) 
	            prev = prev->next;
            newNode->next = prev->next;
            prev->next    = newNode;
            if (!newNode->next)
	            tail = newNode;
        }
        sz++;
    }

    // O(n) — traverse to node before target
    void deleteAt(int index) {
        assert(index >= 0 && index < sz);
        Node* toDelete;
        if (index == 0) {
            toDelete = head;
            head = head->next;
            if (!head) 
	            tail = nullptr;
        } else {
            Node* prev = head;
            for (int i = 0; i < index - 1; i++) 
	            prev = prev->next;
            toDelete = prev->next;
            prev->next = toDelete->next;
            if (!prev->next) 
	            tail = prev;
        }
        delete toDelete;
        sz--;
    }

    int  length() const { return sz; }
    bool empty()  const { return sz == 0; }

    void print() const {
        Node* curr = head;
        while (curr) {
            cout << curr->data;
            if (curr->next) cout << " → ";
            curr = curr->next;
        }
        cout << " → NULL\n";
    }
};

int main() {
    LinkedList list;
    list.append(10);
    list.append(20);
    list.append(30);
    list.print();           // 10 → 20 → 30 → NULL

    list.insertAt(1, 15);
    list.print();           // 10 → 15 → 20 → 30 → NULL

    list.deleteAt(2);
    list.print();           // 10 → 15 → 30 → NULL

    cout << list.at(1) << "\n"; // 15
}
```

---

## 5.2 Variations of Linked Lists

---

> 📘 **YOUR NOTES**
> 
> - **Doubly Linked List:** Each node has both a `next` and a `prev` pointer.
> 
> ```
> NULL ← [A0] ⇄ [A1] ⇄ [A2] ⇄ [A3] → NULL
> ```
> 
> - **Circular Linked List:** The tail's next points back to the head.
> 
> ```
> [A0] → [A1] → [A2] → [A3] → (back to A0)
> ```

---

### 📖 Explanation / Expansion

**Doubly Linked List Advantages:**

- Can traverse in both directions (forward and backward)
- `deleteAt` can be O(1) if you have a pointer to the node (no need to find the previous node)
- Makes `pop_back` O(1) — just update `tail = tail->prev`

**Doubly Linked List Node:**

```cpp
struct DNode {
    int    data;
    DNode* next;
    DNode* prev;
    DNode(int d) : data(d), next(nullptr), prev(nullptr) {}
};
```

**Circular Linked List Use Cases:**

- Round-robin scheduling in OS (each process gets a turn)
- Multiplayer game turns
- Music playlists on repeat
- Buffer management (ring buffer)

---

### 💻 Code Examples

**Doubly Linked List — focused on deletion improvement**

```cpp
struct DNode {
    int    data;
    DNode* prev;
    DNode* next;
    DNode(int d) : data(d), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    DNode* head;
    DNode* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void append(int val) {
        DNode* n = new DNode(val);
        if (!tail) { 
	        head = tail = n; 
		}
        else {
            n->prev    = tail;
            tail->next = n;
            tail       = n;
        }
    }

    // O(1) — direct node pointer, no traversal needed!
    void deleteNode(DNode* node) {
        if (node->prev) 
	        node->prev->next = node->next;
        else            
	        head = node->next;
        
        if (node->next) 
	        node->next->prev = node->prev;
        else            
	        tail = node->prev;
        delete node;
    }

    // O(1) — unlike singly linked list where this is O(n)
    void popBack() {
        if (tail) 
	        deleteNode(tail);
    }
};
```

---

## 5.3 List STL

---

> 📘 **YOUR NOTES**
> 
> - `list<T>` — doubly-linked list implementation.
> - Functions: `clear`, `push_back`, `pop_back`, `push_front`, `pop_front`, `empty`.

---

### 📖 Explanation / Expansion

`std::list<T>` is STL's doubly-linked list. It provides O(1) insertion and deletion anywhere if you have an iterator to the position. But it lacks random access (`v[i]` doesn't work).

**Additional useful `std::list` functions:**

```cpp
list<int> L;
L.sort();              // O(n log n) — mergesort adapted for linked lists
L.reverse();           // O(n)
L.remove(val);         // Remove all occurrences of val — O(n)
L.splice(it, otherL);  // Move all elements from otherL into L at position it — O(1)
L.merge(otherL);       // Merge two sorted lists — O(n)
```

**`std::forward_list<T>`** — singly linked list (C++11). Even more memory-efficient than `list` but only forward traversal.

---

### 🔗 References / What to Review

- **Prerequisites:** Pointers, `struct`, dynamic memory
- **Strongly connected:** Trees (tree nodes use same pointer-based structure), hash tables with chaining, graph adjacency lists
- **STL:** `std::list`, `std::forward_list`

---

### 🌍 Real-World Application

- **OS process lists:** The Linux kernel uses doubly-linked lists (`list_head`) extensively in its scheduler and driver subsystems
- **Browser DOM:** The document tree is a linked structure
- **Text editors:** Gap buffer or rope data structure (based on linked list principles) for efficient editing
- **Memory allocators:** Free list tracking uses linked lists

---

### 🧩 Practice Tasks

#### **Easy:** 
Print a linked list in reverse without modifying it (hint: recursion or stack).
```cpp
void print_reserved() const {
	Node *curr = head;
	Stack<int> stk;
	while (curr)
	{
		stk.push(curr->data);
		curr = curr->next;
	}
	while (!stk.empty())
	{
		cout << stk.top();
		if (stk.size() > 1)
			cout << " → ";
		stk.pop();
	}
}
```
#### **Medium:** 
Detect if a linked list has a cycle (Floyd's Tortoise and Hare algorithm).

```cpp
bool hasCycle(Node* head) {
    Node *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) 
	        return true;
    }
    return false;
}
```
#### **Hard:** 
Reverse a linked list in-place in O(n) time and O(1) space. Then reverse it in groups of k.

```cpp
// reverse in-place
void reverse()
{
	Node *prev = nullptr;
	Node *curr = head;
	while (curr)
	{
		Node *next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	tail = head;
	head = prev;
}
```
```cpp
// reverse in groups
class LinkedList {
private:
	// code
	Node *reverse_k_group(Node *head, int k)
	{
		if (!head || k == 1)
			return head;
		Node *check = head;
		int count = 0;
		while (check && count < k)
		{
			check = check->next;
			count++;
		}
		if (count < k)
			return head;
		Node *prev = nullptr;
		Node *curr = head;
		for (int i = 0; i < k; i++)
		{
			Node *next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		head->next = reverse_k_group(curr, k);
		return prev;
	}
public:
	//code
	void reverse_k_group(int k)
	{
		head = reverse_k_group(head, k);
		Node *curr = head;
		while (curr && curr->next)
		{
			curr = curr->next;
		}
		tail = curr;
	}
	// code
};
```
#### **Competitive:** 
Merge k sorted linked lists into one sorted list in O(n log k) time using a min-heap (priority queue). This is a classic interview problem at top tech companies.
```cpp
void merge_k_lists(vector<LinkedList *> &lists) // O(n log k)
{
	auto compare = [](Node *a, Node *b)
	{
		return a->data > b->data;
	};
	priority_queue<Node *, vector<Node *>, decltype(compare)> minHeap(compare);
	
	// push all heads
	for (LinkedList *list : lists)
		if (list->head)
			minHeap.push(list->head);
	
	// reset this list
	head = nullptr;
	tail = nullptr;
	sz = 0;
	  
	while (!minHeap.empty())
	{
		Node *smallest = minHeap.top();
		minHeap.pop();
		Node *nextNode = smallest->next; // save before nulling
		smallest->next = nullptr;
		if (!tail)
			head = tail = smallest;
		else
		{
			tail->next = smallest;
			tail = smallest;
		}
		sz++;
		
		if (nextNode)
			minHeap.push(nextNode);
	}
	for (LinkedList *list : lists)
		list->clear_without_delete();
}
```
---

### ⚡ Extra Insights

- **Cache performance:** Arrays are far faster in practice than their theoretical O-complexity suggests because of CPU cache lines (contiguous memory = cache-friendly). A linked list traversal is O(n) in theory but may be 10x slower than an array traversal due to cache misses. This is why `std::vector` often outperforms `std::list` even for insertions in the middle.
- **The `prev` pointer cost:** A doubly-linked list uses 2× the pointer memory of a singly-linked list. For large datasets with small node values, this overhead is significant.
- **Sentinel nodes (dummy head/tail):** Simplify insertion/deletion code by eliminating special cases for empty list, inserting at head, or inserting at tail. Many professional implementations use them.

---

# Quick Reference Summary

## Complexity Table

|Operation|Array List|Linked List|Notes|
|---|---|---|---|
|`at(i)`|O(1)|O(n)|Array wins — direct index|
|`append`|O(1)*|O(1)|*amortized; LL needs tail ptr|
|`insertFront`|O(n)|O(1)|LL wins|
|`insertAt(i)`|O(n)|O(n)|Both require traversal|
|`deleteFront`|O(n)|O(1)|LL wins|
|`deleteAt(i)`|O(n)|O(n)|Array: shift; LL: traverse|
|Memory|Compact|Extra ptrs|Array wins for cache|

## Stack vs. Queue

|Property|Stack|Queue|
|---|---|---|
|Ordering|LIFO|FIFO|
|Insert|`push` (top)|`enqueue` (back)|
|Remove|`pop` (top)|`dequeue` (front)|
|STL|`std::stack`|`std::queue`|
|Use case|Undo, recursion, DFS|Scheduling, BFS|

## Big-O Cheat Sheet

|Name|Notation|Example|
|---|---|---|
|Constant|O(1)|Array access, hash lookup|
|Logarithmic|O(log n)|Binary search|
|Linear|O(n)|Linear search, traversal|
|Linearithmic|O(n log n)|Merge sort, heap sort|
|Quadratic|O(n²)|Bubble sort, nested loops|
|Exponential|O(2ⁿ)|Brute-force subset enumeration|

---

_Document last updated: based on Lectures 1–5 of the Data Structures course._ _Textbook reference: "Data Structures and Algorithm Analysis in C++" — Mark Allen Weiss, 4th Edition._