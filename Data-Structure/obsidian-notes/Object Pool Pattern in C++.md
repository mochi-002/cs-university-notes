> **Context:** This is an advanced C++ design pattern built on top of [[Lecture 1 — Classes & Pointers]] concepts. Read that note first if pointers or placement `new` are unfamiliar.

---

## What Is an Object Pool?

An Object Pool is a **creational design pattern** that pre-allocates a fixed number of objects on the heap at startup, then _dispenses_ and _reclaims_ them via `acquire()` / `release()` methods — without ever calling `new` or `delete` in the using code.

**The core trade-off:** spend memory upfront, save time at runtime.

---

## Why It Exists — The Problem

Every time you call `new`, three expensive things happen:

1. **System call** to the OS memory allocator (slow — often microseconds)
2. **Heap fragmentation** — allocations scatter across RAM, leaving unusable gaps
3. **Cache misses** — scattered objects are cold in CPU cache on next access

In performance-critical systems (game engines, network servers, OS kernels), this cost is unacceptable when objects are created and destroyed thousands of times per second.

```
Without pool — heap after many new/delete cycles:
[ Bullet | gap | gap | Bullet | gap | Bullet | gap | gap ]
            ↑ wasted, fragmented, cache-hostile
```

```
With pool — always contiguous:
[ Bullet | Bullet | Bullet | Bullet | Bullet | Bullet ]
  ↑ compact, predictable, cache-friendly
```

---

## The Free List — The Clever Bit

The pool tracks available slots using a **linked list embedded inside the free slots themselves**. When a slot is free, its memory is reused to store a pointer to the _next_ free slot — zero extra memory cost.

```
freeHead
   ↓
[slot 0: next→slot2] ← free
[slot 1: your object] ← IN USE
[slot 2: next→slot4] ← free
[slot 3: your object] ← IN USE
[slot 4: next→null]  ← free
```

- `acquire()` = grab `freeHead`, advance `freeHead = freeHead->next` → **O(1)**
- `release(p)` = `p->next = freeHead`, `freeHead = p` → **O(1)**

---

## Three C++ Concepts You Must Know First

### 1. Placement `new`

Construct an object at a _specific address you already own_ — no allocation:

```cpp
char buffer[sizeof(T)];        // raw memory you own
T* obj = new (buffer) T();     // construct T at that address
obj->~T();                     // must explicitly destroy — no delete!
```

### 2. `alignas(T)`

Tells the compiler: "this memory block must have the same alignment requirement as T." Without it, a `float` or `double` could land at an odd address → undefined behaviour or crash on some CPUs.

```cpp
alignas(T) unsigned char storage[sizeof(T)]; // safe raw storage for any T
```

### 3. `reinterpret_cast<Slot*>(ptr)`

A `union` places the object and the `next` pointer at the **same address**. When releasing an object, you recover the `Slot*` by casting the `T*` back — valid because they share the same memory.

---

## Stage 1 — Simplest Possible Pool

```cpp
#include <iostream>
#include <cassert>
using namespace std;

template <typename T, int N>
class ObjectPool {
private:
    // Union: same memory is either a T (when used) or a Slot* (when free)
    union Slot {
        T     object;   // in-use: the actual object
        Slot* next;     // free: pointer to next free slot

        Slot()  : next(nullptr) {}
        ~Slot() {}
    };

    Slot  slots[N];   // ONE contiguous block, allocated once
    Slot* freeHead;   // head of the free list

public:
    ObjectPool() {
        // Link all slots into a free list at startup
        for (int i = 0; i < N - 1; i++)
            slots[i].next = &slots[i + 1];
        slots[N - 1].next = nullptr;
        freeHead = &slots[0];
    }

    T* acquire() {
        assert(freeHead != nullptr && "Pool exhausted!");
        Slot* slot = freeHead;
        freeHead   = freeHead->next;
        new (&slot->object) T();     // placement new — construct in-place
        return &slot->object;
    }

    void release(T* ptr) {
        ptr->~T();                   // explicit destructor call
        Slot* slot  = reinterpret_cast<Slot*>(ptr);
        slot->next  = freeHead;
        freeHead    = slot;
    }

    bool empty() const { return freeHead == nullptr; }
};
```

---

## Stage 2 — With Constructor Arguments (Perfect Forwarding)

The real version passes any constructor arguments through:

```cpp
template <typename T, int N>
class ObjectPool {
private:
    union Slot {
        alignas(T) unsigned char storage[sizeof(T)]; // safe raw storage
        Slot* next;
        Slot()  : next(nullptr) {}
        ~Slot() {}
    };

    Slot  slots[N];
    Slot* freeHead;

public:
    ObjectPool() {
        for (int i = 0; i < N - 1; i++)
            slots[i].next = &slots[i + 1];
        slots[N - 1].next = nullptr;
        freeHead = &slots[0];
    }

    // Variadic template + perfect forwarding = any constructor signature
    template <typename... Args>
    T* acquire(Args&&... args) {
        assert(freeHead && "Pool is full!");
        Slot* s  = freeHead;
        freeHead = freeHead->next;
        return new (&s->storage) T(forward<Args>(args)...);
    }

    void release(T* ptr) {
        ptr->~T();
        Slot* s  = reinterpret_cast<Slot*>(ptr);
        s->next  = freeHead;
        freeHead = s;
    }

    bool full()  const { return freeHead == nullptr; }
};
```

---

## Stage 3 — Complete Working Example (Game Bullets)

```cpp
#include <iostream>
#include <cassert>
using namespace std;

// ── The Object ──────────────────────────────────────────────────────────────
struct Bullet {
    float x, y;
    float speed;
    bool  active;

    Bullet(float x, float y, float spd)
        : x(x), y(y), speed(spd), active(true) {
        cout << "  [Bullet born]  x=" << x << " y=" << y << "\n";
    }

    ~Bullet() {
        cout << "  [Bullet died]  x=" << x << " y=" << y << "\n";
    }

    void update() { x += speed; }
};

// ── The Pool ────────────────────────────────────────────────────────────────
template <typename T, int N>
class ObjectPool {
private:
    union Slot {
        alignas(T) unsigned char storage[sizeof(T)];
        Slot* next;
        Slot()  : next(nullptr) {}
        ~Slot() {}
    };

    Slot  slots[N];
    Slot* freeHead;
    int   usedCount = 0;

public:
    ObjectPool() {
        for (int i = 0; i < N - 1; i++)
            slots[i].next = &slots[i + 1];
        slots[N - 1].next = nullptr;
        freeHead = &slots[0];
    }

    template <typename... Args>
    T* acquire(Args&&... args) {
        assert(freeHead && "Pool is full!");
        Slot* s  = freeHead;
        freeHead = freeHead->next;
        ++usedCount;
        return new (&s->storage) T(forward<Args>(args)...);
    }

    void release(T* ptr) {
        assert(ptr && "Cannot release null");
        ptr->~T();
        Slot* s  = reinterpret_cast<Slot*>(ptr);
        s->next  = freeHead;
        freeHead = s;
        --usedCount;
    }

    int  used()     const { return usedCount; }
    int  capacity() const { return N; }
    bool full()     const { return freeHead == nullptr; }
};

// ── Demo ─────────────────────────────────────────────────────────────────────
int main() {
    ObjectPool<Bullet, 5> pool;

    cout << "--- Firing bullets ---\n";
    Bullet* b1 = pool.acquire(0.0f, 10.0f, 2.5f);
    Bullet* b2 = pool.acquire(0.0f, 20.0f, 3.0f);
    Bullet* b3 = pool.acquire(0.0f, 30.0f, 1.5f);

    cout << "\nPool used: " << pool.used() << " / " << pool.capacity() << "\n";

    cout << "\n--- b1 goes off-screen, return it ---\n";
    pool.release(b1);
    b1 = nullptr;

    cout << "\nPool used: " << pool.used() << " / " << pool.capacity() << "\n";

    cout << "\n--- Fire new bullet (reuses b1's old slot) ---\n";
    Bullet* b4 = pool.acquire(0.0f, 40.0f, 4.0f);

    cout << "\n--- Cleanup ---\n";
    pool.release(b2);
    pool.release(b3);
    pool.release(b4);

    cout << "\nFinal pool used: " << pool.used() << "\n";
}
```

### Expected Output

```
--- Firing bullets ---
  [Bullet born]  x=0 y=10
  [Bullet born]  x=0 y=20
  [Bullet born]  x=0 y=30

Pool used: 3 / 5

--- b1 goes off-screen, return it ---
  [Bullet died]  x=0 y=10

Pool used: 2 / 5

--- Fire new bullet (reuses b1's old slot) ---
  [Bullet born]  x=0 y=40     ← same memory b1 used!

--- Cleanup ---
  [Bullet died]  x=0 y=20
  [Bullet died]  x=0 y=30
  [Bullet died]  x=0 y=40

Final pool used: 0
```

---

## Complexity Analysis

|Operation|Time|Space|Notes|
|---|---|---|---|
|Pool construction|O(N)|O(N)|One-time cost|
|`acquire()`|**O(1)**|0 extra|Just pointer manipulation|
|`release()`|**O(1)**|0 extra|Just pointer manipulation|
|`new` / `delete` (no pool)|O(1) avg but unpredictable|—|OS allocator overhead|

---

## Common Mistakes

```cpp
// ❌ WRONG: delete instead of release
Bullet* b = pool.acquire(1, 2, 3);
delete b;  // DISASTER — frees memory the pool owns → undefined behaviour

// ❌ WRONG: using object after release
pool.release(b);
b->update();  // b's memory is now part of the free list → undefined behaviour

// ✅ CORRECT: null after release
pool.release(b);
b = nullptr;

// ❌ WRONG: pool destructs while objects are still acquired
// Their destructors never run → resource leak
// Fix: release all acquired objects before the pool goes out of scope
```

---

## Real-World Use Cases

|System|What Gets Pooled|Why|
|---|---|---|
|Game engines (Unity, Unreal)|Bullets, particles, enemies|Spawned/destroyed 1000s/sec|
|Web servers (Node, Nginx)|Database connections|Opening a DB connection costs ~100ms|
|Linux kernel|Memory pages, inodes|`kmem_cache` is an object pool|
|Network stacks|Packet buffers|Millions of packets/sec|
|JVM / .NET runtime|Thread objects|Thread creation is expensive|

---

## Extra Insights

**Why not just use `std::vector`?** `vector` is also contiguous, but `push_back` / `erase` don't give you O(1) arbitrary-slot reclaim with zero fragmentation. The pool's free list solves exactly that.

**Thread safety:** This implementation is NOT thread-safe. In a multi-threaded context, protect `freeHead` with a `std::mutex` or use an atomic free list (lock-free pool — an advanced topic).

**Fixed size limitation:** The pool size `N` is fixed at compile time. If you need a dynamic pool, use a `std::vector<Slot>` and resize (with care — pointers into the pool become invalid on resize).

**STL alternative:** `std::pmr::pool_resource` (C++17) is the standard library's version of this pattern. For production code, consider using it instead of a hand-rolled pool.

---

## Practice Tasks

**Easy:** Add a `used()` and `available()` method that return live counts.

**Medium:** Add bounds checking so `release()` asserts that the pointer actually belongs to this pool (hint: check if it's within `&slots[0]` and `&slots[N-1]`).

**Hard:** Make the pool thread-safe using `std::mutex`. Then benchmark it against a `std::mutex`-protected `new`/`delete` to measure the speedup.

**Competitive:** Implement a _dynamic_ object pool that starts with N slots and doubles capacity when full (like `std::vector`) — without invalidating existing pointers.

---

## References / What to Review

- **Prerequisites:** [[Lecture 1 — Classes & Pointers]] — pointers, `new`/`delete`, constructors/destructors
- **Connected concepts:** Templates (Lecture 3), RAII, Rule of Three/Five
- **C++ features used:** `union`, `alignas`, placement `new`, variadic templates, `std::forward`
- **Further reading:** _Effective C++_ by Scott Meyers — Item 49 (custom allocators); _Game Programming Patterns_ by Robert Nystrom — Object Pool chapter (free online)
- **Standard library:** `std::pmr::pool_resource` (C++17) — the standardised version of this pattern