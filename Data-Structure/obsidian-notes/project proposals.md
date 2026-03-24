# mochi-002 — Project Proposals

> **Role:** Senior C++ Architect session  
> **Stack:** C++20 · Linux · Hyprland  
> **Goal:** Bridge Competitive Programming → System Development

---

## Overview

|#|Project|Core DS|Ships As|Lectures|
|---|---|---|---|---|
|01|`synaptic`|Graph + MinHeap|ncurses TUI|9, 10, 11|
|02|`lexicon`|AVL BST + Hash Table|readline REPL|7, 8|
|03|`herald`|MinHeap + Hash Map|Hyprland daemon|1, 8, 11|

---

## Project 01 — `synaptic`

### A terminal graph navigator

---

### The "Why"

Connects directly to **Lectures 9 & 10**. The `unordered_map<string, list<pair<string,int>>>` adjacency list you studied is literally the storage backbone. Dijkstra's algorithm with a `MinHeapPQ` (Lecture 11 heap) is the query engine. You built both — now you ship them as a navigable TUI.

---

### The Core Logic

- **Graph storage:** adjacency list — `unordered_map<V, list<pair<V, int>>>`
- **Query engine:** Dijkstra's SSSP driven by a hand-rolled `MinHeapPQ<pair<int,V>>`
- **Traversal:** BFS/DFS for connected component discovery
- **UI layer:** `ncurses` TUI — type source/destination, watch the highlighted path animate
- **I/O:** load graph from a text file or pipe via stdin; reload on `SIGHUP`

---

### Class Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    «interface»                           │
│  IDataSource                    IRenderer               │
│  ─────────────                  ──────────              │
│  load(path): bool               render(): void          │
│  reload(): void                 highlight(path): void   │
└──────────┬──────────────────────────────┬───────────────┘
           │ implements (dashed)          │ implements
           ▼                             ▼
┌──────────────────────┐    ┌────────────────────────┐
│  «template»          │    │  «template»            │
│  GraphEngine<V>      │    │  MinHeapPQ<T>          │
│  ──────────────      │    │  ──────────────        │
│  concept Hashable V  │    │  concept Comparable T  │
│  ──────────────      │    │  ──────────────        │
│  dijkstra(src, dst)  │    │  push(val): void       │
│  bfs(start): vec     │    │  pop(): void           │
│  dfs(start): vec     │    │  top(): T&             │
│  addEdge(u,v,w)      │    │  siftUp(i)             │
│  load() override     │    │  siftDown(i)           │
└──────────┬───────────┘    └────────────┬───────────┘
           │ owns                        │ used by
           └─────────────┬──────────────┘
                         ▼
           ┌─────────────────────────┐
           │  DijkstraRunner         │
           │  ───────────────        │
           │  run(src, dst)          │
           │  holds GraphEngine ref  │
           │  drives MinHeapPQ       │
           └────────────┬────────────┘
                        │ owns
           ┌────────────▼────────────┐
           │  MapViewRenderer        │──────► ncurses WINDOW*
           │  ───────────────────    │
           │  render(): void         │
           │  highlight(path): void  │
           │  setStatusLine(msg)     │
           └────────────┬────────────┘
                        │
           ┌────────────▼────────────┐
           │  App                   │
           │  ───                   │
           │  unique_ptr<IDataSrc>  │
           │  unique_ptr<IRenderer> │
           │  run() — event loop    │
           └────────────────────────┘
```

---

### C++20 Features

**Concepts** — pin the template requirement at the declaration site, not buried in a compiler error 40 lines deep:

```cpp
// Before: unconstrained — explodes at instantiation
template <typename V>
class GraphEngine { /* ... */ };

// After: constrained — clean error at call site
template <typename V>
concept Hashable = requires(V v) {
    { hash<V>{}(v) } -> convertible_to<size_t>;
};

template <Hashable V>
class GraphEngine { /* ... */ };
```

**Ranges** — flatten the nested adjacency iteration:

```cpp
// Before: nested loops
for (auto& [v, neighbours] : adj)
    for (auto& [n, w] : neighbours)
        process(n, w);

// After: flat pipeline
auto edges = adj | views::values | views::join;
for (auto& [n, w] : edges) process(n, w);
```

---

### Initial Header Structure

```cpp
// synaptic/include/GraphEngine.h
#pragma once
#include <concepts>
#include <unordered_map>
#include <list>
#include <optional>
#include <vector>
#include <string>
using namespace std;

template <typename V>
concept Hashable = requires(V v) {
    { hash<V>{}(v) } -> convertible_to<size_t>;
};

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> convertible_to<bool>;
};

// ── Interfaces ───────────────────────────────────────────────────
struct IDataSource {
    virtual bool load(const string& path) = 0;
    virtual void reload()                 = 0;
    virtual ~IDataSource()                = default;
};

struct IRenderer {
    virtual void render()                          = 0;
    virtual void highlight(const vector<string>&)  = 0;
    virtual ~IRenderer()                           = default;
};

// ── Core engine ──────────────────────────────────────────────────
template <Hashable V>
class GraphEngine : public IDataSource {
public:
    using Weight     = int;
    using PathResult = optional<pair<Weight, vector<V>>>;

    void        addEdge(const V& from, const V& to, Weight w,
                        bool directed = false);
    PathResult  dijkstra(const V& src, const V& dst) const;
    vector<V>   bfs(const V& start) const;
    vector<V>   dfs(const V& start) const;
    int         connectedComponents() const;

    bool load(const string& path) override;
    void reload() override;

private:
    unordered_map<V, list<pair<V, Weight>>> adj_;
    string lastFile_;
};

// ── Min-heap priority queue ──────────────────────────────────────
template <Comparable T>
class MinHeapPQ {
public:
    void     push(const T& val);
    void     pop();
    const T& top() const;
    bool     empty() const;
    int      size()  const;

private:
    vector<T> data_ = {T{}};   // 1-indexed sentinel
    void siftUp(int i);
    void siftDown(int i);
};

// ── TUI renderer ─────────────────────────────────────────────────
class MapViewRenderer : public IRenderer {
public:
    explicit MapViewRenderer(const GraphEngine<string>& engine);
    void render()                             override;
    void highlight(const vector<string>& path) override;
    void setStatusLine(const string& msg);
    ~MapViewRenderer() override;

private:
    const GraphEngine<string>& engine_;
    string statusLine_;
};

// ── App shell ────────────────────────────────────────────────────
class App {
public:
    App(unique_ptr<IDataSource>, unique_ptr<IRenderer>);
    void run();   // keystroke → query → render loop

private:
    unique_ptr<IDataSource> source_;
    unique_ptr<IRenderer>   renderer_;
};
```

---

### Real-World Connection

Every GPS app (Google Maps, OSRM) is this architecture at scale: an adjacency list of road segments, Dijkstra or A* on a priority queue, a rendering layer on top. You're building the same thing, minus the map tiles.

---

### Practice Extension

After the basic version works, try replacing `MinHeapPQ` with `std::priority_queue` via a template parameter — the `IIndex`-style swap forces you to think about what the heap's interface contract actually is.

---

---

## Project 02 — `lexicon`

### A dual-index command-line dictionary engine

---

### The "Why"

Forces **Lecture 7** (AVL BST, `std::map`) and **Lecture 8** (hash table, `std::unordered_map`) to compete on the same dataset. `lexicon` lets you query the same word list through either index and benchmark them live — you watch O(log n) vs O(1) play out on real data. Range queries ("all words between 'aba' and 'abc'") only the BST can answer. Exact lookups go to the hash. Both implement the same `IIndex<K,V>` interface.

---

### The Core Logic

- **`BSTIndex<K,V>`:** AVL self-balancing tree — O(log n) for all ops, supports ordered range queries
- **`HashIndex<K,V>`:** Separate chaining — O(1) average exact lookup
- **`LexiconStore`:** Owns both, routes by query type — range → BST, exact → hash
- **`ReplShell`:** `readline`-powered REPL, wire into a Hyprland scratchpad keybind
- **Benchmark mode:** `bench <word> 10000` — prints ns/op for both indexes side by side

---

### Class Architecture

```
┌──────────────────────────────────────────────────────────────┐
│                        «interface»                           │
│              IIndex<K, V>   (requires Indexable<K>)          │
│              ─────────────────────────────────────           │
│              insert(key, val): void                          │
│              lookup(key): optional<V>                        │
│              remove(key): bool                               │
│              rangeQuery(lo, hi): vector<pair<K,V>>           │
│              size(): size_t                                  │
└────────────────────┬─────────────────────┬───────────────────┘
                     │ implements           │ implements
                     ▼                     ▼
        ┌────────────────────┐  ┌────────────────────┐
        │  «template»        │  │  «template»        │
        │  BSTIndex<K,V>     │  │  HashIndex<K,V>    │
        │  ──────────────    │  │  ──────────────    │
        │  AVL-balanced      │  │  separate chaining │
        │  rotateLeft/Right  │  │  rehash(newBuckets)│
        │  balanceFactor()   │  │  loadFactor()      │
        │  O(log n) ordered  │  │  O(1) avg exact    │
        │  range queries ✓   │  │  range queries ✗   │
        └─────────┬──────────┘  └──────────┬─────────┘
                  │ owned by               │ owned by
                  └────────────┬───────────┘
                               ▼
               ┌───────────────────────────┐
               │  LexiconStore             │
               │  ─────────────            │
               │  lookup(word) → hash      │
               │  range(lo, hi) → BST      │
               │  bulkInsert(span<>)       │
               │  benchmark(word, N)       │
               └──────────────┬────────────┘
                              │ held by
                              ▼
               ┌───────────────────────────┐
               │  ReplShell                │
               │  ─────────────            │
               │  run(): void              │
               │  parse cmd → dispatch     │
               │  readline history         │
               └───────────────────────────┘
```

---

### C++20 Features

**Composite Concept** — `Indexable<K>` combines hashability AND ordering in one constraint, since the BST needs `<` and the hash needs `hash<K>{}`:

```cpp
template <typename K>
concept Indexable =
    requires(K a, K b) {
        { hash<K>{}(a) } -> convertible_to<size_t>;
        { a < b }        -> convertible_to<bool>;
        { a == b }       -> convertible_to<bool>;
    };
```

**`std::span`** — zero-copy bulk insert, no raw pointer + length pairs:

```cpp
// Before
void bulkInsert(const pair<string,string>* data, size_t n);

// After — non-owning view, bounds-safe, accepts vector/array/C-array
void bulkInsert(span<const pair<string,string>> entries);
```

---

### Initial Header Structure

```cpp
// lexicon/include/Index.h
#pragma once
#include <concepts>
#include <functional>
#include <optional>
#include <vector>
#include <list>
#include <string>
#include <span>
using namespace std;

// ── Composite concept ────────────────────────────────────────────
template <typename K>
concept Indexable =
    requires(K a, K b) {
        { hash<K>{}(a) } -> convertible_to<size_t>;
        { a < b }        -> convertible_to<bool>;
        { a == b }       -> convertible_to<bool>;
    };

// ── Shared interface ─────────────────────────────────────────────
template <Indexable K, typename V>
struct IIndex {
    virtual void              insert(const K& key, const V& val)        = 0;
    virtual optional<V>       lookup(const K& key) const                = 0;
    virtual bool              remove(const K& key)                      = 0;
    virtual vector<pair<K,V>> rangeQuery(const K& lo, const K& hi) const = 0;
    virtual size_t            size() const                              = 0;
    virtual void              clear()                                   = 0;
    virtual ~IIndex()                                                   = default;
};

// ── AVL BST index ────────────────────────────────────────────────
template <Indexable K, typename V>
class BSTIndex : public IIndex<K, V> {
public:
    void              insert(const K&, const V&)        override;
    optional<V>       lookup(const K&) const            override;
    bool              remove(const K&)                  override;
    vector<pair<K,V>> rangeQuery(const K&, const K&) const override;
    size_t            size()  const                     override;
    void              clear()                           override;
    int               height() const;

private:
    struct Node {
        K     key;
        V     val;
        int   height = 1;
        Node* left   = nullptr;
        Node* right  = nullptr;
    };
    Node*  root_  = nullptr;
    size_t count_ = 0;

    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* rebalance(Node* n);
    int   getHeight(Node* n) const;
    int   balanceFactor(Node* n) const;
};

// ── Hash index (separate chaining) ──────────────────────────────
template <Indexable K, typename V>
class HashIndex : public IIndex<K, V> {
public:
    explicit HashIndex(size_t buckets = 53);   // prime default

    void              insert(const K&, const V&)        override;
    optional<V>       lookup(const K&) const            override;
    bool              remove(const K&)                  override;
    vector<pair<K,V>> rangeQuery(const K&, const K&) const override;
    size_t            size()  const                     override;
    void              clear()                           override;

    float loadFactor() const;
    void  rehash(size_t newBuckets);

private:
    using Chain = list<pair<K, V>>;
    vector<Chain> table_;
    size_t        count_ = 0;
    size_t        hashKey(const K&) const;
};

// ── Orchestrator ─────────────────────────────────────────────────
class LexiconStore {
public:
    LexiconStore();
    void loadFromFile(const string& path);
    void bulkInsert(span<const pair<string, string>> entries);

    optional<string>            lookup(const string& word) const;
    vector<pair<string,string>> range(const string& lo,
                                      const string& hi) const;
    void benchmark(const string& word, int iterations = 10000) const;

private:
    BSTIndex<string,string>  bst_;
    HashIndex<string,string> hash_;
};

// ── REPL shell ───────────────────────────────────────────────────
class ReplShell {
public:
    explicit ReplShell(LexiconStore& store);
    void run();   // readline loop: parse → dispatch → print

private:
    LexiconStore& store_;
    void handleCommand(const string& line);
};
```

---

### Real-World Connection

This is exactly how search engines work internally: a hash map for O(1) exact URL lookup, a sorted B-tree index for range queries on timestamps or scores. You're building both and learning precisely when to reach for each.

---

### Practice Extension

Add a `--mode bst|hash|auto` flag. In `auto` mode, the shell detects whether the query contains wildcards or a range operator and routes accordingly. This forces you to think about query planning — what databases do before executing SQL.

---

---

## Project 03 — `herald`

### A priority-based system event dispatcher for Hyprland

---

### The "Why"

Everything from the course converges here. The `MinHeapPQ` (Lecture 11) drives the scheduling loop exactly like a real OS process scheduler — lowest priority integer fires first. The `HandlerRegistry` is a direct port of your Lecture 8 hash table — `unordered_map<string, vector<IEventHandler*>>`. The handler chain exercises virtual dispatch and the OOP hierarchy from Lecture 1. And it ships as a real daemon that sits in your Hyprland `exec-once` and dispatches events from a Unix socket.

---

### The Core Logic

- **`Event` struct:** has `priority`, `timestamp`, `tag` (routing key), `payload`. C++20 `operator<=>` gives all comparisons for free
- **`EventQueue`:** a `MinHeapPQ<Event>` — lowest-priority-int fires first, timestamp breaks ties
- **`HandlerRegistry`:** `unordered_map<string, vector<shared_ptr<IEventHandler>>>` — maps event tags to handler chains
- **`IEventHandler`:** pure virtual interface — `NotifyHandler` fires `libnotify`, `LogHandler` writes to file, `ScriptHandler` runs a shell command
- **`Dispatcher`:** pops min event, looks up tag in registry, calls all registered handlers. Runs on a `jthread` — auto-joins on destruction, no manual cleanup
- **`HeraldDaemon`:** opens a Unix socket, reads events from `heraldctl send battery.low "15%"`, feeds them to the dispatcher

---

### Class Architecture

```
                  ┌────────────────────────────┐
                  │         «aggregate»        │
                  │  Event                     │
                  │  ──────                    │
                  │  priority:  int            │
                  │  timestamp: long           │
                  │  tag:       string         │
                  │  payload:   string         │
                  │  operator<=>(Event&)       │  ← C++20 spaceship
                  └────────────────────────────┘

┌────────────────────┐          ┌──────────────────────────┐
│  EventQueue        │          │       «interface»         │
│  ──────────        │          │  IEventHandler            │
│  MinHeapPQ<Event>  │          │  ─────────────            │
│  push(ev): void    │          │  handle(Event&): void     │
│  popMin(): Event   │          │  name(): string_view      │
│  peekMin(): Event& │          └──────────┬───────────────┘
└─────────┬──────────┘    implements       │
          │               ┌────────────────┼────────────────┐
          │               ▼                ▼                ▼
          │  ┌─────────────────┐ ┌──────────────┐ ┌───────────────┐
          │  │ NotifyHandler   │ │ LogHandler   │ │ ScriptHandler │
          │  │ ─────────────── │ │ ──────────── │ │ ───────────── │
          │  │ libnotify popup │ │ file/syslog  │ │ shell exec    │
          │  └─────────────────┘ └──────────────┘ └───────────────┘
          │
          │        ┌──────────────────────────────────────┐
          │        │  HandlerRegistry                     │
          │        │  ───────────────                     │
          │        │  unordered_map<string,               │
          │        │      vector<shared_ptr<IEventHandler>>>│
          │        │  subscribe(tag, handler)             │
          │        │  unsubscribe(tag, name)              │
          │        │  dispatch(ev): void                  │
          │        └──────────────────────────────────────┘
          │                        │
          └────────────┬───────────┘
                       ▼
          ┌────────────────────────────────┐
          │  Dispatcher                   │
          │  ──────────                   │
          │  postEvent(ev): void          │
          │  tick(): void                 │   ← popMin → dispatch
          │  runAsync(): void             │   ← starts jthread
          │  optional<jthread> worker_    │   ← auto-joins on dtor
          └───────────────┬──────────────┘
                          │
          ┌───────────────▼──────────────┐
          │  HeraldDaemon                │
          │  ────────────                │
          │  loadConfig(toml): void      │
          │  run(): void                 │   ← unix socket loop
          │  EventQueue     queue_       │
          │  HandlerRegistry registry_   │
          │  Dispatcher     dispatcher_  │
          └──────────────────────────────┘
```

---

### C++20 Features

**`operator<=>`** — the spaceship operator gives `<`, `>`, `<=`, `>=`, `==`, `!=` all at once from one declaration. The heap comparison just works:

```cpp
struct Event {
    int    priority  = 0;
    long   timestamp = 0;
    string tag;
    string payload;

    auto operator<=>(const Event& o) const noexcept {
        if (priority != o.priority)
            return priority <=> o.priority;
        return timestamp <=> o.timestamp;   // tie-break: earlier fires first
    }
    bool operator==(const Event&) const = default;
};
```

**`jthread` + `stop_token`** — the background dispatch loop shuts down cleanly without a manual `join()` or a raw `atomic<bool>` flag:

```cpp
// Before: raw thread with flag
atomic<bool> running_ = true;
thread worker_([this] { while (running_) tick(); });
// ... somewhere else:
running_ = false;
worker_.join();   // easy to forget

// After: jthread auto-joins, stop_token is cooperative
optional<jthread> worker_;

void runAsync() {
    worker_.emplace([this](stop_token tok) {
        while (!tok.stop_requested()) tick();
    });
}
// Destructor fires → jthread destructs → requests stop → joins. Done.
```

---

### Initial Header Structure

```cpp
// herald/include/Herald.h
#pragma once
#include <compare>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
using namespace std;

// ── Event ────────────────────────────────────────────────────────
struct Event {
    int    priority  = 0;
    long   timestamp = 0;
    string tag;
    string payload;

    auto operator<=>(const Event& o) const noexcept {
        if (priority != o.priority) return priority <=> o.priority;
        return timestamp <=> o.timestamp;
    }
    bool operator==(const Event&) const = default;
};

// ── Handler interface ────────────────────────────────────────────
struct IEventHandler {
    virtual void        handle(const Event& ev) = 0;
    virtual string_view name() const            = 0;
    virtual ~IEventHandler()                    = default;
};

// ── Concrete handlers ────────────────────────────────────────────
struct NotifyHandler : IEventHandler {
    void        handle(const Event& ev) override;   // libnotify
    string_view name() const override { return "notify"; }
};

struct LogHandler : IEventHandler {
    explicit LogHandler(const string& path);
    void        handle(const Event& ev) override;   // append to file
    string_view name() const override { return "log"; }
private:
    string logPath_;
};

struct ScriptHandler : IEventHandler {
    explicit ScriptHandler(const string& cmd);
    void        handle(const Event& ev) override;   // system(cmd)
    string_view name() const override { return "script"; }
private:
    string cmd_;
};

// ── EventQueue ───────────────────────────────────────────────────
class EventQueue {
public:
    void         push(const Event& ev);
    Event        popMin();
    const Event& peekMin() const;
    bool         empty() const;
    int          size()  const;

private:
    vector<Event> heap_ = {Event{}};   // 1-indexed
    void siftUp(int i);
    void siftDown(int i);
};

// ── HandlerRegistry ──────────────────────────────────────────────
class HandlerRegistry {
public:
    void subscribe(const string& tag,
                   shared_ptr<IEventHandler> handler);
    void unsubscribe(const string& tag, string_view handlerName);
    void dispatch(const Event& ev) const;

private:
    unordered_map<string,
        vector<shared_ptr<IEventHandler>>> map_;
};

// ── Dispatcher ───────────────────────────────────────────────────
class Dispatcher {
public:
    Dispatcher(EventQueue& q, const HandlerRegistry& reg);
    void postEvent(const Event& ev);
    void tick();        // popMin → dispatch one event
    void runAsync();    // launch jthread, stop on destruction
    void stop();

private:
    EventQueue&            queue_;
    const HandlerRegistry& registry_;
    optional<jthread>      worker_;   // auto-joins on ~Dispatcher
};

// ── Daemon entry point ───────────────────────────────────────────
class HeraldDaemon {
public:
    explicit HeraldDaemon(const string& socketPath);
    void loadConfig(const string& tomlPath);
    void run();   // open unix socket → read events → Dispatcher

private:
    string          socketPath_;
    EventQueue      queue_;
    HandlerRegistry registry_;
    Dispatcher      dispatcher_{queue_, registry_};
};
```

---

### Real-World Connection

This is structurally identical to how `systemd`'s event loop works, how `libuv` (Node.js's backend) dispatches I/O callbacks, and how game engines handle their entity component system update queues. The `MinHeapPQ` + hash registry pattern is everywhere at the system level.

### Hyprland Integration

```bash
# ~/.config/hypr/hyprland.conf
exec-once = herald --socket /tmp/herald.sock --config ~/.config/herald/config.toml

# Send events from anywhere:
heraldctl send battery.low "14%" --priority 1
heraldctl send window.focus "kitty" --priority 5
```

---

### Practice Extension

Add **event expiry** — each `Event` gets an optional `ttl_ms`. The dispatcher checks `now() - timestamp > ttl_ms` before calling handlers and silently drops stale events. This turns `herald` into a proper real-time scheduler and forces you to think about the monotonic clock and `chrono`.

---

---

## Unified C++20 Upgrade Map

A quick reference for migrating your current template patterns across all three projects:

|Before|After|Benefit|
|---|---|---|
|`template <typename T>` (unconstrained)|`template <Hashable T>` (concept)|Compile-time error at call site, not buried in instantiation|
|`vector<T>&` bulk param|`span<const T>`|Zero-copy, bounds-safe, accepts any contiguous range|
|`thread` + `atomic<bool>` + manual `join`|`jthread` + `stop_token`|Auto-joins, cooperative cancellation, no leaks|
|`if (a < b && b == c)` chained|`operator<=>` + `= default`|One declaration gives all six comparisons|
|Nested `for` over nested containers|`views::values|views::join`|
|`T* ptr; if (found) ptr = &x;`|`optional<T>`|Explicit nullable, no sentinel values, no raw null|

---

## Build Setup (all three projects)

```cmake
# CMakeLists.txt (shared pattern)
cmake_minimum_required(VERSION 3.20)
project(synaptic CXX)                     # or lexicon / herald

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)     # for clangd / LSP

add_executable(synaptic src/main.cpp)
target_include_directories(synaptic PRIVATE include)

# Project-specific links:
# synaptic: target_link_libraries(synaptic ncurses)
# lexicon:  target_link_libraries(lexicon  readline)
# herald:   target_link_libraries(herald   notify)
```

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j$(nproc)
```

---

_References: DS Learning Resource Parts 1 & 2 — Lectures 1, 7, 8, 9, 10, 11_  
_C++ standard: ISO/IEC 14882:2020 (C++20)_  
_Target environment: Linux, Hyprland, GCC 13+ or Clang 16+_