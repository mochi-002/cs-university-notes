> **Continuation of:** `[[Data Structures — Complete Learning Resource (Part 1)]]` (Lectures 1–5) This file covers Lectures 6–11: BSTs, Hash Tables, Graphs, Priority Queues / Heaps, and Debugging.

---

# Table of Contents

1. [[#Lecture 6 — Binary Search Trees I]]
2. [[#Lecture 7 — Binary Search Trees II & STL]]
3. [[#Lecture 8 — Hash Tables]]
4. [[#Lecture 9 — Graphs I]]
5. [[#Lecture 10 — Graphs II & Shortest Path]]
6. [[#Lecture 11 — Priority Queues, Heaps & Debugging]]
7. [[#Comprehensive Practice Task Bank]]

---

# Lecture 6 — Binary Search Trees I

---

## 6.1 Tree Terminology

---

> 📘 **YOUR NOTES**
> 
> - **Root:** the single top node — every tree has exactly one root.
> - **Edge:** a link between two nodes (n nodes → exactly n−1 edges).
> - **Parent / Child / Siblings / Leaf nodes.**
> - **Subtree:** a node and all its descendants.
> - **Height of a node:** edges to its most distant leaf.
> - **Depth (Level) of a node:** edges back up to the root. Root depth = 0.
> - A **leaf** always has height 0; the **root** always has depth 0.

---

### 📖 Explanation / Expansion

**Why trees?** All the data structures so far (arrays, linked lists, stacks, queues) are _linear_ — one dimension. Trees introduce the first _non-linear_ structure, letting us model hierarchies and achieve logarithmic operations.

**Key formula to memorise:** A tree with `n` nodes always has exactly `n − 1` edges. This comes from the fact that every node except the root has exactly one parent edge.

**Height vs. Depth — the common confusion:**

```
         A          ← depth 0, height 3
        / \
       B   C        ← depth 1
      / \
     D   E          ← depth 2
    /
   F                ← depth 3, height 0 (leaf)

Height of A = 3  (longest path downward)
Depth  of F = 3  (path back up to root)
```

**Binary Tree Types — visual cheat sheet:**

|Type|Rule|Notes|
|---|---|---|
|Full|Every node has 0 or 2 children|No node has exactly 1 child|
|Complete|All levels full except last; last level filled left-to-right|Used by heaps|
|Perfect|All internal nodes have 2 children; all leaves at same depth|n = 2^(h+1) − 1 nodes|
|Balanced|Height of left and right subtrees differ by at most 1 for every node|Guarantees O(log n)|

---

## 6.2 Binary Search Trees

---

> 📘 **YOUR NOTES**
> 
> - An important application of binary trees.
> - **BST Property:** For every node X:
>     - All values in the **left** subtree are **smaller** than X.
>     - All values in the **right** subtree are **larger** than X.
> - Average running time of most operations: **O(log N)**.
> - **Node structure:**
> 
> ```cpp
> template <class T>
> class Node {
>     T value;
>     Node<T>* left;
>     Node<T>* right;
> };
> ```

---

### 📖 Explanation / Expansion

**Why O(log N)?** At each step of a search/insert, you eliminate roughly half the tree. After `k` comparisons you've narrowed the search space to `N / 2^k` nodes. Setting that to 1 gives `k = log₂(N)`.

**The critical caveat:** O(log N) is the _average_ case for a randomly shaped tree. If you insert elements in sorted order (e.g., 1, 2, 3, 4, 5...), the BST degenerates into a linked list with O(N) operations. This is why balanced BSTs (AVL, Red-Black) exist.

```
Balanced BST:        Degenerate BST (sorted insert):
      5                1
     / \                \
    3   7                2
   / \ / \                \
  2  4 6  8                3
                            \
                             4  ← O(N) to reach
```

**BST Invariant — the full statement:**

> For EVERY node X in the tree, ALL nodes in X's left subtree have values strictly less than X's value, and ALL nodes in X's right subtree have values strictly greater than X's value. This must hold recursively for every subtree, not just immediate children.

This subtle point trips up many students: `[2 is left child of 3 is left child of 6]` → 2 must be less than 6, not just less than 3. The invariant applies globally.

---

### 💻 Code Examples

**BST Node and basic Contains / Insert**

```cpp
#include <iostream>
#include <algorithm> // for max
using namespace std;

template <typename T>
struct Node {
    T     value;
    Node* left;
    Node* right;

    Node(T val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BST {
private:
    Node<T>* root;

    // ── Recursive helpers ──────────────────────────────────────────────────
    bool containsHelper(Node<T>* node, T val) const {
        if (!node) return false;
        if (val == node->value) return true;
        if (val  < node->value) return containsHelper(node->left,  val);
        else                    return containsHelper(node->right, val);
    }

    Node<T>* insertHelper(Node<T>* node, T val) {
        if (!node) return new Node<T>(val);   // found insertion point
        if      (val < node->value) node->left  = insertHelper(node->left,  val);
        else if (val > node->value) node->right = insertHelper(node->right, val);
        // val == node->value: duplicates ignored (BST property)
        return node;
    }

    void inOrderHelper(Node<T>* node) const {
        if (!node) return;
        inOrderHelper(node->left);
        cout << node->value << " ";
        inOrderHelper(node->right);
    }

    void preOrderHelper(Node<T>* node) const {
        if (!node) return;
        cout << node->value << " ";
        preOrderHelper(node->left);
        preOrderHelper(node->right);
    }

    void postOrderHelper(Node<T>* node) const {
        if (!node) return;
        postOrderHelper(node->left);
        postOrderHelper(node->right);
        cout << node->value << " ";
    }

    void destroyTree(Node<T>* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    int heightHelper(Node<T>* node) const {
        if (!node) return -1; // empty tree has height -1
        return 1 + max(heightHelper(node->left), heightHelper(node->right));
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroyTree(root); }

    bool contains(T val)  const { return containsHelper(root, val); }
    void insert(T val)          { root = insertHelper(root, val); }
    void inOrder()   const { inOrderHelper(root);   cout << "\n"; }
    void preOrder()  const { preOrderHelper(root);  cout << "\n"; }
    void postOrder() const { postOrderHelper(root); cout << "\n"; }
    int  height()    const { return heightHelper(root); }
};

int main() {
    BST<int> tree;
    // Insert: 100, 200, 90, 150, 125, 88, 99, 210 (lecture exercise)
    for (int v : {100, 200, 90, 150, 125, 88, 99, 210})
        tree.insert(v);

    tree.inOrder();   // 88 90 99 100 125 150 200 210 ← sorted!
    tree.preOrder();  // 100 90 88 99 200 150 125 210
    tree.postOrder(); // 88 99 90 125 150 210 200 100

    cout << "Contains 125: " << tree.contains(125) << "\n"; // 1
    cout << "Contains 999: " << tree.contains(999) << "\n"; // 0
    cout << "Height: "       << tree.height()      << "\n"; // 3
}
```

---

## 6.3 BST Operations — Contains & Insert

---

> 📘 **YOUR NOTES**
> 
> - **Contains(55):** Start at root (60). 55 < 60 → go left (41). 55 > 41 → go right (53). 55 > 53 → go right. Found 55.
> - **Insert(48):** Navigate like Contains. At each node compare and go left/right until you reach a null pointer, then insert there.
> - **Exercise:** Insert 100, 200, 90, 150, 125, 88, 99, 210 in order.

---

### 📖 Explanation / Expansion

**Contains — Iterative version (sometimes preferred over recursive in competitive programming):**

```cpp
bool contains(Node<T>* root, T val) {
    Node<T>* curr = root;
    while (curr) {
        if      (val == curr->value) return true;
        else if (val  < curr->value) curr = curr->left;
        else                         curr = curr->right;
    }
    return false;
}
```

**Why does in-order traversal produce sorted output?** By the BST invariant, for any node X: everything in left subtree < X < everything in right subtree. In-order visits left first, then X, then right — this naturally produces ascending order. This is why BSTs are used as sorted containers.

---

## 6.4 Tree Traversals

---

> 📘 **YOUR NOTES**
> 
> - **In-order (Left → Root → Right):** Prints sorted list for BST.
> - **Pre-order (Root → Left → Right):** Useful for displaying folder structures; creates a copy of the tree.
> - **Post-order (Left → Right → Root):** Useful for deleting tree nodes (children before parent); evaluating expression trees.
> - For tree: `63, 41, 16, 25, 53, 46, 55, 74, 65, 64, 70`:
>     - In-order: 16, 25, 41, 46, 53, 55, 63, 64, 65, 70, 74
>     - Pre-order: 63, 41, 16, 25, 53, 46, 55, 74, 65, 64, 70
>     - Post-order: 25, 16, 46, 55, 53, 41, 64, 70, 65, 74, 63

---

### 📖 Explanation / Expansion

**Expression Trees — the real use of pre/post-order:** The expression `(a * b + c * d) + (e * f * g)` maps to a tree where operators are internal nodes and operands are leaves.

- **In-order** gives infix (needs parentheses)
- **Pre-order** gives prefix / Polish notation: `+ + * a b * c d * * e f g`
- **Post-order** gives postfix / Reverse Polish: `a b * c d * + e f * g * +` — exactly what stack-based evaluators use (from Lecture 2!)

**Memory trick:**

```
Pre  = ROOT first  (PRE-fix)
In   = ROOT middle (IN-fix)
Post = ROOT last   (POST-fix)
```

**Level-order (BFS) traversal** — not in your notes but critical: Visit nodes level by level, left to right. Uses a queue. Gives: `63 41 74 16 53 65 25 46 55 64 70`

```cpp
void levelOrder(Node<T>* root) {
    if (!root) return;
    queue<Node<T>*> q;
    q.push(root);
    while (!q.empty()) {
        Node<T>* n = q.front(); q.pop();
        cout << n->value << " ";
        if (n->left)  q.push(n->left);
        if (n->right) q.push(n->right);
    }
}
```

---

### 🔗 References / What to Review

- **Prerequisites:** Pointers, recursion, linked list node structure
- **Strongly connected:** Recursion (every BST operation is naturally recursive), stacks (DFS), queues (BFS/level-order)
- **Textbook:** Chapter 4 sections 4.1, 4.2, 4.3

---

### 🌍 Real-World Application

- **Databases:** B-Trees (generalisation of BST with many children per node) are the index structure behind every SQL database
- **File systems:** Directory trees in every OS
- **Compilers:** Abstract Syntax Trees (AST) — the expression tree your notes show is exactly this
- **Game AI:** Decision trees for NPC behaviour

---

### 🧩 Practice Tasks

**Easy:** Write `findMin()` and `findMax()` for a BST (hint: go left/right until null).

**Medium:** Write `isBST(Node* root, int min, int max)` that verifies whether a binary tree satisfies the BST property. The naive approach (check only immediate children) is _wrong_ — you must pass valid ranges down.

**Hard:** Given a sorted array, build a **balanced** BST from it (height = ⌊log₂n⌋). This is the inverse of in-order traversal and requires picking the middle element as root recursively.

**Competitive:** Given a BST, find the **k-th smallest element** in O(log N + k) time without storing all elements. (Hint: augment each node with a subtree size count.)

---

# Lecture 7 — Binary Search Trees II & STL

---

## 7.1 BST Remove

---

> 📘 **YOUR NOTES**
> 
> - **Three cases when removing a node:**
>     1. **Remove a leaf:** Set the parent's pointer to that child to `nullptr`.
>     2. **Remove a node with one child:** Replace the node with its child (bypass it).
>     3. **Remove a node with two children:**
>         - Find the **smallest value in the right subtree** (in-order successor).
>         - Replace the deleted node's value with the successor's value.
>         - Delete the successor node (which has at most one child).
> - **Helper:** `FindParent(value)` — traverse with two pointers `a` (current) and `b` (parent), advance both until `a->value == target`, return `b`.

---

### 📖 Explanation / Expansion

**Why the in-order successor?** The in-order successor (smallest in right subtree) is the value that would come immediately after the deleted node in sorted order. Replacing with it maintains the BST invariant:

- Everything in left subtree < successor < everything in right subtree.

Alternatively you can use the **in-order predecessor** (largest in left subtree) — both are correct.

**Finding the in-order successor:** Go right once, then go left until you can't. That left-most node is the smallest in the right subtree.

```cpp
Node<T>* findMin(Node<T>* node) {
    while (node->left) node = node->left;
    return node;
}
```

**Remove — complete recursive implementation:**

```cpp
Node<T>* removeHelper(Node<T>* node, T val) {
    if (!node) return nullptr;   // value not found

    if (val < node->value) {
        node->left = removeHelper(node->left, val);
    } else if (val > node->value) {
        node->right = removeHelper(node->right, val);
    } else {
        // Found the node to delete
        if (!node->left && !node->right) {
            // Case 1: leaf
            delete node;
            return nullptr;
        } else if (!node->left) {
            // Case 2a: only right child
            Node<T>* temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            // Case 2b: only left child
            Node<T>* temp = node->left;
            delete node;
            return temp;
        } else {
            // Case 3: two children — find in-order successor
            Node<T>* successor = findMin(node->right);
            node->value = successor->value;           // copy successor's value
            node->right = removeHelper(node->right, successor->value); // delete it
        }
    }
    return node;
}
```

---

### ⚡ Extra Insights

**Lazy deletion:** In some applications, instead of physically removing nodes, you mark them as "deleted" with a boolean flag. Operations skip marked nodes. This avoids the complexity of pointer manipulation and is useful when deletions are rare but searches are frequent. The cost: storage grows without bound unless occasionally cleaned up.

---

## 7.2 BST Complexity & Balanced Trees

---

> 📘 **YOUR NOTES**
> 
> - A degenerate (skewed) BST has **O(n)** complexity — equivalent to a linked list.
> - **Solution:** Balanced Binary Search Trees — guaranteed O(log n) height for n items.
> - **1. Red-Black Trees:**
>     - Every node is red or black.
>     - Root is always black.
>     - No two adjacent red nodes.
>     - Every root-to-leaf path has the same number of black nodes.
> - **2. AVL Trees (Adelson-Velsky and Landis):**
>     - For every node: |height(left) − height(right)| ≤ 1.
>     - Maintained via **rotations** when insertions violate the balance condition.

---

### 📖 Explanation / Expansion

**AVL Rotations — the 4 cases:** When inserting into an AVL tree breaks balance at some ancestor node `z`:

|Case|Shape|Fix|
|---|---|---|
|Left-Left|z → y → x (all left)|Single right rotation at z|
|Right-Right|z → y → x (all right)|Single left rotation at z|
|Left-Right|z → y left, y → x right|Left rotate at y, then right rotate at z|
|Right-Left|z → y right, y → x left|Right rotate at y, then left rotate at z|

**Red-Black vs AVL:**

- **AVL trees** are more strictly balanced → faster lookups (better for read-heavy)
- **Red-Black trees** allow slightly more imbalance → faster insertions/deletions (better for write-heavy)
- `std::map` and `std::set` in most STL implementations use Red-Black trees

---

## 7.3 BST in STL — `set` and `map`

---

> 📘 **YOUR NOTES**
> 
> - **`set<T>`:** Stores unique, sorted elements. Backed by a balanced BST (Red-Black Tree). Iterating gives sorted order.
> 
> ```cpp
> set<int> s;
> s.insert(100);
> set<int>::iterator it;
> for (it = s.begin(); it != s.end(); it++) cout << *it;
> ```
> 
> - **`pair<T1, T2>`:** Couples two values. Access via `.first` and `.second`. Create with `make_pair(x, y)`.
> - **`map<Key, Value>`:** Key-value store, sorted by key, backed by BST. Access/insert via `[]` operator.
> 
> ```cpp
> map<string,string> jobs;
> jobs["Ahmed"] = "Engineer";
> cout << jobs["Ahmed"];
> ```
> 
> - **`map` functions:** `at(k)`, `begin()`, `end()`, `empty()`, `find(k)`, `insert(pair)`, `erase(k)`.
> - **`map` iterator:** `it->first` (key), `it->second` (value).

---

### 📖 Explanation / Expansion

**`set` vs `unordered_set`, `map` vs `unordered_map`:**

|Container|Backing|Lookup|Insert|Ordered?|
|---|---|---|---|---|
|`set`|Red-Black Tree|O(log n)|O(log n)|Yes (sorted)|
|`unordered_set`|Hash Table|O(1) avg|O(1) avg|No|
|`map`|Red-Black Tree|O(log n)|O(log n)|Yes (by key)|
|`unordered_map`|Hash Table|O(1) avg|O(1) avg|No|

**When to use which:** If you need elements in sorted order, use `set`/`map`. If you only need fast lookup and don't care about order, use the `unordered_` variants (next lecture).

**`map::operator[]` quirk:** If the key doesn't exist, `[]` **inserts a default-constructed value**. This can create unintended entries. Use `find()` to safely check without inserting:

```cpp
auto it = jobs.find("Ahmed");
if (it != jobs.end())
    cout << it->second;  // safe — key exists
```

---

### 💻 Code Examples

```cpp
#include <map>
#include <set>
#include <iostream>
using namespace std;

int main() {
    // ── set ────────────────────────────────────────────────────────────────
    set<int> s = {5, 3, 8, 1, 3, 5};  // duplicates silently ignored
    for (int x : s) cout << x << " "; // 1 3 5 8  ← auto-sorted
    cout << "\n";

    cout << "Count 3: " << s.count(3) << "\n"; // 1
    cout << "Count 7: " << s.count(7) << "\n"; // 0

    // ── map ────────────────────────────────────────────────────────────────
    map<string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"]   = 82;
    scores["Carol"] = 91;

    // Iterates in sorted key order
    for (auto& [name, score] : scores)       // C++17 structured bindings
        cout << name << ": " << score << "\n";
    // Alice: 95 | Bob: 82 | Carol: 91

    // Safe lookup
    if (scores.find("Dave") == scores.end())
        cout << "Dave not found\n";

    // Erase
    scores.erase("Bob");
    cout << "Size: " << scores.size() << "\n"; // 2
}
```

---

### 🔗 References / What to Review

- **Strongly connected:** Red-Black Tree rotations, AVL height proofs, hash tables (next lecture)
- **Textbook:** Chapter 4 sections 4.3, 4.8

---

### 🧩 Practice Tasks

**Easy:** Use `map<string, int>` to count word frequencies in a sentence.

**Medium:** Given a BST, find the **lowest common ancestor (LCA)** of two nodes. The LCA is the deepest node that has both targets as descendants. (Hint: use the BST property — if both values are less than current, go left; if both greater, go right; otherwise current is the LCA.)

**Hard:** Implement a BST with **augmented subtree sizes** (`int size` per node). Use it to implement `rank(val)` (how many elements are less than val) and `select(k)` (find the k-th smallest element), both in O(log N).

**Competitive:** Given a stream of integers, maintain a data structure that can answer "what is the median so far?" after each insertion in O(log N). (Classic: use a max-heap for the lower half and a min-heap for the upper half.)

---

# Lecture 8 — Hash Tables

---

## 8.1 The Map ADT & Motivation

---

> 📘 **YOUR NOTES**
> 
> - Maps store key-value (k, v) pairs. No duplicate keys.
> - The key acts as a unique identifier / address for the object.
> - **Operations:** `size`, `isEmpty`, `get(k)`, `put(k,v)`, `remove(k)`.
> - **Implementations and their complexity:**
>     - Linked list: O(n) for all operations.
>     - BST: O(log n) average.
>     - **Hash Table: O(1) average!**

---

### 📖 Explanation / Expansion

**The core idea:** Instead of searching through elements (O(n)) or comparing positions in a sorted structure (O(log n)), use the key itself to _compute_ exactly where the value should be stored. This gives O(1) lookup.

```
Key "Ahmed" → hash function → index 3 → arr[3] = 25000
```

The hash function converts a key (any type) into an integer index into a fixed array.

---

## 8.2 Hash Functions

---

> 📘 **YOUR NOTES**
> 
> - **Hash Function:** maps key → position within the array.
> - **Hash Value:** the result.
> - **Collision:** two keys map to the same index.
> - **Load Factor (λ):** `n / T` — number of stored elements divided by table size.
> - **A good hash function:**
>     - Easy to compute.
>     - Distributes keys uniformly.
>     - Returns a value within table bounds.
>     - Always returns the same value for the same key.
>     - Minimises collisions.
> - **For integers:** `h(Key) = Key % TableSize` — best when TableSize is prime.
> - **Folding method:** Split key into chunks, add them, mod by TableSize. (e.g., phone 436-555-4601 → 43+65+55+46+01 = 210 → 210 % T)
> - **String hashing:** Use polynomial rolling hash with prime base (e.g., 37).
> - **Perfect hashing:** No collisions. Minimal perfect: no wasted slots.

---

### 📖 Explanation / Expansion

**Why prime TableSize?** If `TableSize` has factors in common with typical key patterns, some buckets will be systematically overloaded. A prime table size guarantees better distribution regardless of the key distribution.

**Polynomial rolling hash for strings:**

```cpp
int hash(const string& s, int tableSize) {
    long long h = 0;
    for (char c : s)
        h = (h * 37 + c) % tableSize;
    return (int)h;
}
```

Each character's contribution is multiplied by a higher power of 37, so "abc" and "bca" produce different hashes.

**Why 37?** It's prime, larger than the ASCII alphabet size (26), and small enough not to overflow quickly. The modulo keeps the running hash in bounds.

---

## 8.3 Collision Resolution

---

> 📘 **YOUR NOTES**
> 
> - **1. Separate Chaining (Closed Addressing):** Each slot holds a linked list of all keys hashing to that index.
>     - Advantages: Handles unlimited collisions, no prior size knowledge needed, graceful degradation.
>     - Disadvantages: Sequential search within chains, extra pointer memory.
> - **2. Open Addressing (Probing):** On collision, find another slot within the same array.
>     - **2A. Linear Probing:** `NewHash(Key) = [Hash(Key) + i] % T` — try next slot, then next, etc.
>         - Problem: **Clustering** — long runs of occupied cells form, slowing future operations.
>     - **2B. Quadratic Probing:** `NewHash(Key) = [Hash(Key) + i²] % T` — jump 1, 4, 9, 16... slots.
>         - Reduces primary clustering.
>     - **2C. Double Hashing:** `NewHash(Key) = [Hash(Key) + i * Hash2(Key)] % T`
>         - Hash2 must never return 0. Table size must be prime.
>         - Avoids clustering entirely.
> - **Deletion with open addressing:** Mark slots as "deleted" (tombstone) rather than empty to avoid breaking probe chains.

---

### 📖 Explanation / Expansion

**Separate Chaining visualised:**

```
index 0: → NULL
index 1: → [Ahmed: 25000] → NULL
index 2: → [Amal: 45300] → [Hussein: 15000] → NULL  ← collision!
index 3: → NULL
```

**Tombstone deletion problem:** When you delete with linear probing, you can't just set the slot to empty because that breaks probe chains:

```
Insert A → slot 3
Insert B → collision → slot 4 (linear probe)
Delete A → slot 3 becomes empty
Search B → reach slot 3, see EMPTY, stop → WRONG: B is at slot 4!

Fix: Mark slot 3 as DELETED (tombstone)
Search B → reach slot 3, see TOMBSTONE, continue → reach slot 4, find B ✓
Insert C → reuse slot 3 (tombstone) ✓
```

**Load factor guidance:**

- λ < 0.5: all strategies perform well
- λ = 0.7: performance starts degrading
- λ > 0.7: rehash the table (double size, re-insert everything)

**Rehashing:**

```
1. Create new table of size ~2 * oldSize (next prime)
2. For every occupied slot in old table:
       insert(key, value) into new table
3. Delete old table
Cost: O(n) — but rare enough that amortised cost stays O(1)
```

---

### 💻 Code Examples

**Hash table with separate chaining (from scratch)**

```cpp
#include <iostream>
#include <list>
#include <vector>
#include <string>
using namespace std;

template <typename K, typename V>
class HashTable {
private:
    int tableSize;
    vector<list<pair<K, V>>> table;

    int hashCode(const K& key) const {
        // Generic: use hash<K> from STL
        return (int)(hash<K>{}(key) % tableSize);
    }

public:
    HashTable(int size = 11) : tableSize(size), table(size) {}

    void put(const K& key, const V& val) {
        int idx = hashCode(key);
        // Check if key already exists
        for (auto& p : table[idx]) {
            if (p.first == key) { p.second = val; return; }
        }
        table[idx].push_back({key, val});
    }

    bool get(const K& key, V& outVal) const {
        int idx = hashCode(key);
        for (const auto& p : table[idx])
            if (p.first == key) { outVal = p.second; return true; }
        return false;
    }

    void remove(const K& key) {
        int idx = hashCode(key);
        table[idx].remove_if([&](const pair<K,V>& p){ return p.first == key; });
    }

    float loadFactor() const {
        int count = 0;
        for (auto& chain : table) count += chain.size();
        return (float)count / tableSize;
    }
};

int main() {
    HashTable<string, string> ht;
    ht.put("Ahmed",   "Engineer");
    ht.put("Salma",   "Doctor");
    ht.put("Mohamed", "Teacher");

    string job;
    if (ht.get("Salma", job)) cout << "Salma: " << job << "\n"; // Doctor
    ht.remove("Ahmed");
    cout << "Load factor: " << ht.loadFactor() << "\n";
}
```

**STL `unordered_map`**

```cpp
#include <unordered_map>
#include <iostream>
using namespace std;

int main() {
    unordered_map<string, int> freq;

    string words[] = {"apple", "banana", "apple", "cherry", "banana", "apple"};
    for (const auto& w : words) freq[w]++;

    for (auto& [word, count] : freq)
        cout << word << ": " << count << "\n";
    // Order is not guaranteed (no sorting, unlike map)

    cout << "Load factor: " << freq.load_factor() << "\n";
    cout << "Bucket count: " << freq.bucket_count() << "\n";
}
```

---

### 🔗 References / What to Review

- **Prerequisites:** Arrays, linked lists, modular arithmetic, prime numbers
- **Strongly connected:** `map` vs `unordered_map`, amortised analysis
- **Textbook:** Chapter 5

---

### 🌍 Real-World Application

- **Compilers:** Symbol tables (variable name → memory address) use hash tables
- **Databases:** Hash joins, index structures
- **Caching:** CPU L1/L2 caches use set-associative hashing; web caches (Redis) are giant hash tables
- **Python `dict` / Java `HashMap`:** Hash tables under the hood
- **DNS lookups:** Hostname → IP address mapping is a hash table

---

### 🧩 Practice Tasks

**Easy:** Use `unordered_map<char, int>` to find the first non-repeating character in a string.

**Medium:** Implement a function that given an array of integers and a target sum, returns all pairs that sum to the target. Solve it in O(n) using a hash set.

**Hard:** Implement `LRU Cache` — a cache of size k that evicts the least recently used item when full. Support `get(key)` and `put(key, value)` both in O(1). (Hint: `unordered_map` + doubly linked list.)

**Competitive:** Given a string, find the length of the longest substring without repeating characters. O(n) using a sliding window + hash map. (Classic LeetCode #3)

---

# Lecture 9 — Graphs I

---

## 9.1 Graph Fundamentals

---

> 📘 **YOUR NOTES**
> 
> - A graph is the most general data structure — allows arbitrary relationships.
> - **G = (V, E):** V = set of vertices, E = set of edges.
> - An edge is a pair (v, w) where v, w ∈ V. Edges may have a **weight/cost**.
> - **Degree:** number of edges from a vertex. In directed graphs: **in-degree** (edges pointing in) and **out-degree** (edges going out).
> - **Path:** sequence of vertices w₁, w₂, ..., wₙ where each consecutive pair is connected by an edge. **Length** = number of edges.
> - **Simple path:** no repeated vertices.
> - **Cycle:** path that starts and ends at the same vertex.
> - **Connected graph (undirected):** path exists between every pair of vertices.
> - **Directed graphs:** **strongly connected** (path between any pair in both directions); **weakly connected** (connected ignoring direction).
> - **Complete graph:** edge between every pair of vertices. |E| = |V|(|V|-1)/2.
> - **Trees as graphs:** directed, acyclic, one path from root to every node.

---

### 📖 Explanation / Expansion

**Dense vs. Sparse graphs:**

- **Dense:** |E| ≈ |V|² — nearly every pair of vertices is connected. Use adjacency matrix.
- **Sparse:** |E| ≈ |V| — few edges relative to vertices. Use adjacency list.

Most real-world graphs (social networks, road maps) are sparse. A road map of a city has millions of nodes but each intersection connects to only ~4 roads on average.

**Complexity notation:** Graph algorithms are expressed in terms of both |V| (vertices) and |E| (edges), e.g., O(V + E).

---

## 9.2 Graph Representation

---

> 📘 **YOUR NOTES**
> 
> - **1. Adjacency Matrix:** 2D array `A[V][V]`. `A[i][j] = 1` (or weight) if edge (i,j) exists.
>     - Fast edge lookup: O(1).
>     - Space: O(V²) — wasteful for sparse graphs.
>     - Slow to iterate over all edges: O(V²).
> - **2. Adjacency List:** Each vertex stores a list of its neighbours.
>     - Space: O(V + E) — efficient for sparse graphs.
>     - Fast to iterate over edges: O(E).
>     - Edge lookup: O(degree of vertex).
> - **STL suggested implementation:** `unordered_map<string, list<pair<string, int>>>` — key is vertex label, value is list of (neighbour, weight) pairs.

---

### 💻 Code Examples

**Graph with adjacency list**

```cpp
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
using namespace std;

class Graph {
private:
    unordered_map<string, list<pair<string,int>>> adj;
    bool directed;

public:
    Graph(bool isDirected = false) : directed(isDirected) {}

    void addVertex(const string& v) {
        if (adj.find(v) == adj.end())
            adj[v] = {};
    }

    void addEdge(const string& from, const string& to, int weight = 1) {
        adj[from].push_back({to, weight});
        if (!directed)
            adj[to].push_back({from, weight});
    }

    void print() const {
        for (auto& [v, neighbours] : adj) {
            cout << v << " → ";
            for (auto& [n, w] : neighbours)
                cout << n << "(" << w << ") ";
            cout << "\n";
        }
    }

    // DFS — iterative (uses stack)
    void dfs(const string& start) {
        unordered_set<string> visited;
        stack<string> st;
        st.push(start);
        cout << "DFS: ";
        while (!st.empty()) {
            string node = st.top(); st.pop();
            if (visited.count(node)) continue;
            visited.insert(node);
            cout << node << " ";
            for (auto& [neighbour, _] : adj[node])
                if (!visited.count(neighbour))
                    st.push(neighbour);
        }
        cout << "\n";
    }

    // DFS — recursive
    void dfsRec(const string& node, unordered_set<string>& visited) {
        visited.insert(node);
        cout << node << " ";
        for (auto& [neighbour, _] : adj[node])
            if (!visited.count(neighbour))
                dfsRec(neighbour, visited);
    }

    // BFS — uses queue
    void bfs(const string& start) {
        unordered_set<string> visited;
        queue<string> q;
        q.push(start);
        visited.insert(start);
        cout << "BFS: ";
        while (!q.empty()) {
            string node = q.front(); q.pop();
            cout << node << " ";
            for (auto& [neighbour, _] : adj[node])
                if (!visited.count(neighbour)) {
                    visited.insert(neighbour);
                    q.push(neighbour);
                }
        }
        cout << "\n";
    }
};

int main() {
    Graph g(false); // undirected
    g.addEdge("A", "B");
    g.addEdge("A", "I");
    g.addEdge("B", "I");
    g.addEdge("I", "C");
    g.addEdge("I", "G");
    g.addEdge("C", "D");
    g.addEdge("C", "E");
    g.addEdge("C", "F");
    g.addEdge("E", "H");
    g.addEdge("G", "H");
    g.addEdge("G", "F");

    g.dfs("A"); // A B I C D E H G F
    g.bfs("A"); // A B I C G D E F H
}
```

---

## 9.3 Graph Traversals — DFS and BFS

---

> 📘 **YOUR NOTES**
> 
> - **DFS (Depth First Search):**
>     - Uses a **stack**.
>     - Explores as deep as possible before backtracking.
>     - Algorithm: init all as unvisited → push root → while stack not empty: pop n; if unvisited, mark visited, output, push all unvisited neighbours.
>     - Recursive version: mark visited, recurse on each unvisited neighbour.
>     - Complexity: **O(V + E)**.
> - **BFS (Breadth First Search):**
>     - Uses a **queue**.
>     - Explores all neighbours before going deeper.
>     - Algorithm: init all as unvisited → enqueue root, mark visited → while queue not empty: dequeue n, output, enqueue all unvisited neighbours.
>     - Complexity: **O(V + E)**.
> - **When to use:**
>     - **BFS:** Best when depth varies (finds shortest path in unweighted graphs).
>     - **DFS:** Best when you need to search the entire graph; easier to implement recursively; uses less memory (stack grows to depth × branching, vs BFS queue grows to branching^depth).

---

### 📖 Explanation / Expansion

**DFS = pre-order tree traversal generalised to graphs.** The only difference: graphs can have cycles, so you need a `visited` set to avoid infinite loops.

**BFS shortest path intuition:** BFS visits nodes in order of increasing distance from the source. The first time BFS reaches a node is always via the shortest path (in terms of number of edges, for unweighted graphs).

**Memory comparison:**

```
Graph with branching factor k, depth d:
DFS stack: O(d × k) — linear in depth
BFS queue: O(k^d)   — exponential in depth
```

For deep graphs, DFS uses far less memory. For shallow, wide graphs, BFS is more manageable.

**Connected components:** If a graph is not connected, run DFS/BFS from each unvisited vertex to discover all components:

```cpp
for (auto& [v, _] : adj)
    if (!visited.count(v)) {
        dfsRec(v, visited);  // new component
        cout << "\n";
    }
```

---

### 🌍 Real-World Application

- **DFS:** Topological sorting (build systems, compiler dependencies), cycle detection, maze solving, finding strongly connected components (Kosaraju's algorithm)
- **BFS:** Social network "degrees of separation", web crawler link discovery, GPS shortest path (unweighted), Peer-to-peer network discovery
- **Both:** Garbage collection (mark phase marks all reachable objects from roots)

---

### 🧩 Practice Tasks

**Easy:** Given an adjacency list graph, print all connected components (run DFS/BFS from each unvisited vertex).

**Medium:** Detect if an undirected graph contains a cycle using DFS. Return true/false.

**Hard:** Given a directed graph, perform **topological sort** (order vertices so that for every directed edge u→v, u comes before v). Only possible if no cycle exists (a DAG). Use DFS with a finished-order stack. (Kahn's algorithm using BFS is an alternative.)

**Competitive:** **Number of islands** (LeetCode #200) — given a 2D grid of '1's (land) and '0's (water), count the number of islands. Each island is a group of connected '1's. This is a standard DFS/BFS on a grid graph.

---

# Lecture 10 — Graphs II & Shortest Path

---

## 10.1 Shortest Path Problem

---

> 📘 **YOUR NOTES**
> 
> - **Shortest path problem:** find the minimum-cost path between two vertices.
> - **Applications:** road networks, logistics, communications, power grid analysis, community detection.
> - **Single-Source Shortest Path:** Find shortest path from one source vertex to ALL other vertices.
> - **Dijkstra's Algorithm** solves this for graphs with **non-negative edge weights**.
> - **Why Dijkstra fails for negative weights:** Once a node is marked "known", its distance is finalised. A negative edge discovered later could give a shorter path — but the algorithm has already committed.

---

### 📖 Explanation / Expansion

**Dijkstra's Algorithm — the greedy intuition:** Always extend the shortest known path. Among all "unknown" vertices, pick the one with the smallest current cost, mark it as "known" (its shortest path is now final), and use it to update its neighbours.

**Step-by-step trace (from your notes):**

```
Graph: a→b(2), a→c(4), b→c(1), b→d(4), b→e(2), e→f(2), d→f(0)
Source: a

Step 0: dist = {a:0, b:∞, c:∞, d:∞, e:∞, f:∞}
Step 1: Mark a (cost 0). Update neighbours: b=2, c=4
Step 2: Mark b (cost 2, cheapest unknown). Update: c=min(4,2+1)=3, d=6, e=4
Step 3: Mark c (cost 3). Update: e=min(4,3+3)=4 (no improvement)
Step 4: Mark e (cost 4). Update: f=6
Step 5: Mark d (cost 6). Update: f=min(6,6+0)=6 (no improvement)
Step 6: Mark f (cost 6)

Shortest path a→f: a→b→e→f (cost 6)
```

**Data structure for Dijkstra:** Naively: scan all unknown vertices each step → O(V²). Better: use a **min-heap (priority queue)** → O((V + E) log V). The STL `priority_queue` or a Fibonacci heap is used in practice.

---

### 💻 Code Examples

**Dijkstra's Algorithm with `priority_queue`**

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
using namespace std;

// Graph as adjacency list: adj[u] = list of (v, weight)
void dijkstra(int src, int V,
              const vector<vector<pair<int,int>>>& adj) {

    vector<int> dist(V, INT_MAX);
    vector<int> pred(V, -1);
    // min-heap: {cost, vertex}
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [cost, u] = pq.top(); pq.pop();

        if (cost > dist[u]) continue; // stale entry

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pred[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Print results
    for (int i = 0; i < V; i++) {
        cout << "Vertex " << i << ": dist = " << dist[i]
             << ", pred = " << pred[i] << "\n";
    }
}

int main() {
    int V = 6; // vertices 0=a, 1=b, 2=c, 3=d, 4=e, 5=f
    vector<vector<pair<int,int>>> adj(V);
    // a→b(2), a→c(4), b→c(1), b→d(4), b→e(2), e→f(2)
    adj[0].push_back({1, 2}); // a→b
    adj[0].push_back({2, 4}); // a→c
    adj[1].push_back({2, 1}); // b→c
    adj[1].push_back({3, 4}); // b→d
    adj[1].push_back({4, 2}); // b→e
    adj[4].push_back({5, 2}); // e→f

    dijkstra(0, V, adj);
}
```

---

### 🔗 References / What to Review

- **Prerequisites:** Graphs I, priority queues (Lecture 11), greedy algorithms
- **Related algorithms:** Bellman-Ford (handles negative weights, O(VE)), Floyd-Warshall (all-pairs shortest path, O(V³)), A* (heuristic, used in game AI and GPS)

---

### 🌍 Real-World Application

- **GPS / Google Maps:** Dijkstra / A* on road network graphs (with real-time traffic weights)
- **Network routing:** OSPF routing protocol uses Dijkstra
- **Social networks:** "People you may know" via shortest paths in the social graph
- **Game AI:** Pathfinding in game worlds (A* is Dijkstra + heuristic)

---

### 🧩 Practice Tasks

**Easy:** Trace Dijkstra's algorithm by hand on the graph from lecture. Reconstruct the shortest path from `a` to `f` by back-tracking the predecessor table.

**Medium:** Implement Dijkstra and also recover/print the actual path (not just the distance) using the predecessor array.

**Hard:** Implement **Bellman-Ford** algorithm that handles negative weights (but not negative cycles). Detect if a negative cycle exists. Compare its complexity O(VE) to Dijkstra's O((V+E) log V).

**Competitive:** **Network Delay Time** (LeetCode #743) — given a directed weighted graph and a source node, find the time it takes for all nodes to receive a signal (= maximum shortest path from source). Return -1 if any node is unreachable.

---

# Lecture 11 — Priority Queues, Heaps & Debugging

---

## 11.1 Priority Queue

---

> 📘 **YOUR NOTES**
> 
> - A **priority queue** is a queue where elements are arranged by priority, not arrival order.
> - Higher-priority elements are retrieved/removed first.
> - **Use cases:** print job scheduling (staff first), ER patient triage, multi-tasking OS scheduler, Dijkstra's algorithm.
> - **Operations:** Insertion, Deletion (remove highest priority), Peek/Top.
> - **Implementation complexity comparison:**
> 
> |Implementation|enqueue|dequeue|peek|
> |---|---|---|---|
> |Unsorted array/list|O(1)|O(n)|O(n)|
> |Sorted list|O(n)|O(1)|O(1)|
> |BST|O(log n)–O(n)|O(log n)–O(n)|O(log n)–O(n)|
> |**Heap**|**O(log n)**|**O(log n)**|**O(1)**|

---

### 📖 Explanation / Expansion

**Why heaps win:** Sorted arrays/lists give O(1) for one operation but O(n) for the other. The heap is the data structure that _balances_ both to O(log n), giving the best practical performance for priority queues.

**STL `priority_queue`** uses a max-heap by default:

```cpp
#include <queue>
priority_queue<int> maxPQ;           // max-heap (top = largest)
priority_queue<int, vector<int>,
    greater<int>> minPQ;             // min-heap (top = smallest)
```

---

## 11.2 Heap

---

> 📘 **YOUR NOTES**
> 
> - A **Heap** is a **complete binary tree** where:
>     - **Max-Heap:** every node ≥ its children (root = maximum).
>     - **Min-Heap:** every node ≤ its children (root = minimum).
> - Height of a heap with N nodes: **O(log N)** (because it's a complete binary tree).
> - **Heaps are NOT fully sorted** — no specific relationship between siblings or cousins.
> - **Heap as an array:** A complete binary tree maps perfectly to an array:
>     - `Parent(i) = ⌊i/2⌋`
>     - `Left(i) = 2i`
>     - `Right(i) = 2i + 1` (Using 1-based indexing.)
> - **Insert (bubble up):** Add at the end of the array. Repeatedly swap with parent if larger (max-heap) until heap property restored.
> - **DeleteMin/Max (bubble down):** Replace root with the last element. Repeatedly swap with the larger child until heap property restored.
> - **Build heap from sequence:** `5, 3, 17, 10, 84, 19, 6, 22, 9` → result Min-Heap root = 3.

---

### 📖 Explanation / Expansion

**Array representation — why it works perfectly for complete binary trees:**

```
Tree:        Array (1-indexed):
    1        [_, 1, 2, 3, 4, 5, 6, 7, 8, 9]
   / \         0  1  2  3  4  5  6  7  8  9
  2   3
 / \ / \
4  5 6  7
/ \
8  9

Parent(8) = 8/2 = 4 ✓
Left(3)   = 6   ✓
Right(3)  = 7   ✓
```

This avoids pointer overhead (no `left*`, `right*`, `parent*` per node) — a heap is just a flat array!

**Bubble up (insert) trace — Min-Heap, insert 2:**

```
Before: [3, 5, 6, 9, 84, 19, 17, 22, 10]  (1-indexed)
Append: [3, 5, 6, 9, 84, 19, 17, 22, 10, 2]
Compare 2 vs parent(10) = 5... 2 < 5 → swap
        [3, 5, 6, 9, 84, 19, 17, 22, 2, 10]   ← 2 at index 5
Compare 2 vs parent(5) = 2... wait, 2 == 2 → done? No...
Compare 2 vs parent(5) = 3... 2 < 3 → swap
        [2, 5, 6, 9, 84, 19, 17, 22, 3, 10]  ← 2 is now root
Heap property restored.
```

**Build heap in O(N) — the trick:** Inserting N elements one by one = O(N log N). But starting from the middle and sifting DOWN gives O(N):

```cpp
// Build heap from array arr[1..n]
for (int i = n/2; i >= 1; i--)
    siftDown(arr, i, n);
```

Proof: most nodes are near the bottom and sift very little. The total work sums to O(N).

**Heap Sort:**

1. Build max-heap from array: O(N)
2. Repeatedly extract max (put at end, shrink heap): O(N log N) Total: O(N log N), in-place, no extra memory!

---

### 💻 Code Examples

**Min-Heap from scratch**

```cpp
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

class MinHeap {
private:
    vector<int> data; // 1-indexed (data[0] unused)

    int parent(int i) { return i / 2; }
    int left(int i)   { return 2 * i; }
    int right(int i)  { return 2 * i + 1; }

    void siftUp(int i) {
        while (i > 1 && data[parent(i)] > data[i]) {
            swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int n = data.size() - 1;
        while (left(i) <= n) {
            int smallest = left(i);
            if (right(i) <= n && data[right(i)] < data[smallest])
                smallest = right(i);
            if (data[i] <= data[smallest]) break;
            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    MinHeap() { data.push_back(0); } // sentinel at index 0

    void insert(int val) {
        data.push_back(val);
        siftUp(data.size() - 1);
    }

    int top() const {
        assert(data.size() > 1 && "Empty heap");
        return data[1];
    }

    void pop() {
        assert(data.size() > 1 && "Empty heap");
        data[1] = data.back();
        data.pop_back();
        if (data.size() > 1) siftDown(1);
    }

    bool empty() const { return data.size() == 1; }
    int  size()  const { return data.size() - 1; }

    // Build heap in O(N)
    void buildHeap(const vector<int>& arr) {
        data = {0}; // sentinel
        data.insert(data.end(), arr.begin(), arr.end());
        for (int i = (data.size()-1)/2; i >= 1; i--)
            siftDown(i);
    }
};

int main() {
    MinHeap h;
    for (int v : {5, 3, 17, 10, 84, 19, 6, 22, 9})
        h.insert(v);

    cout << "Min: " << h.top() << "\n"; // 3

    // Extract all in sorted order — heapsort!
    while (!h.empty()) {
        cout << h.top() << " ";
        h.pop();
    }
    // 3 5 6 9 10 17 19 22 84
    cout << "\n";
}
```

**STL `priority_queue`**

```cpp
#include <queue>
#include <iostream>
using namespace std;

int main() {
    // Max-heap (default)
    priority_queue<int> maxPQ;
    for (int v : {5, 3, 17, 10, 84, 19}) maxPQ.push(v);
    cout << maxPQ.top() << "\n"; // 84

    // Min-heap
    priority_queue<int, vector<int>, greater<int>> minPQ;
    for (int v : {5, 3, 17, 10, 84, 19}) minPQ.push(v);
    cout << minPQ.top() << "\n"; // 3

    // Custom comparator for pairs — useful in Dijkstra
    using PII = pair<int,int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    pq.push({10, 1}); // {cost, vertex}
    pq.push({2,  3});
    pq.push({7,  2});
    cout << pq.top().first << "\n"; // 2 — min cost first
}
```

---

### 🔗 References / What to Review

- **Prerequisites:** Complete binary trees, arrays, BSTs
- **Strongly connected:** Dijkstra (uses min-heap), Prim's MST algorithm, Huffman coding
- **STL:** `priority_queue`, `make_heap`, `push_heap`, `pop_heap`

---

### 🌍 Real-World Application

- **OS Task Scheduler:** Processes sorted by priority in a heap
- __Dijkstra / Prim / A_:_* All use min-heaps for their efficiency
- **Huffman Coding:** Builds optimal prefix codes using a min-heap (used in ZIP, JPEG, MP3)
- **Event-driven simulation:** Events sorted by timestamp in a min-heap
- **Top-K problems:** Finding the k largest elements in a stream using a min-heap of size k

---

### 🧩 Practice Tasks

**Easy:** Use STL `priority_queue` to sort an array in O(N log N) by pushing all elements and popping.

**Medium:** Given a stream of integers, maintain the **median** at each step using two heaps (a max-heap for the lower half, a min-heap for the upper half).

**Hard:** Implement **Heap Sort** in-place using your MinHeap class. It should sort without any extra memory. Verify it's O(N log N).

**Competitive:** **Kth Largest Element in an Array** (LeetCode #215) — find the k-th largest element in O(N log k) using a min-heap of size k. (Better than sorting the entire array.)

---

## 11.3 Debugging

---

> 📘 **YOUR NOTES**
> 
> - **Debugging mindset:**
>     1. Pay attention to error messages.
>     2. Google things.
>     3. Explain your logic to another person or a rubber duck.
>     4. Narrow down your problem — understand where the error is generated.
>     5. Take a break and think about something else.
>     6. Write clean code (DRY, KISS, SOLID).
> - **Debugger in Visual Studio:** Enter with F5 (Start Debugging).
> - **Breakpoints:** Click left margin or press F9. Pauses execution at that line.
> - **Step Into (F11):** Execute one statement at a time, entering function calls.
> - **Step Over (F10):** Execute next line without entering function calls.
> - **Run to Cursor:** Execute until the line where cursor is placed.
> - **Data Tips:** Hover over a variable while paused to see its value.
> - **Autos window:** Shows variables on the current and preceding lines.
> - **Locals window:** Shows all variables currently in scope.
> - **Watch window:** Manually add expressions to monitor.
> - **Call Stack window:** Shows the chain of function calls that led to the current point.
> - **Conditional Breakpoint:** Breakpoint that only triggers when a condition is true (e.g., `i == 50`).

---

### 📖 Explanation / Expansion

**The rubber duck technique** is real and effective. Explaining your code aloud forces you to confront assumptions you didn't know you were making. Many bugs are discovered mid-sentence before the duck needs to say anything.

**Debugger vs. print statements:** Print debugging (`cout << "here\n"`) works but is imprecise and leaves residue. A debugger lets you:

- Inspect any variable at any point without modifying the source
- Step through code at human speed to watch state change
- Set conditional breakpoints so you only stop when the bug manifests (e.g., when a value becomes negative)

**Common C++ bugs to watch for:**

```cpp
// 1. Off-by-one in arrays
for (int i = 0; i <= n; i++) arr[i]; // ← should be i < n

// 2. Uninitialized pointer dereference
Node* p;
p->value = 5; // ← p is garbage — crash!

// 3. Null pointer after delete
delete p;
p->value; // ← p still exists but points to freed memory

// 4. Infinite recursion (no base case)
int fact(int n) { return n * fact(n-1); } // ← missing if(n==0) return 1;

// 5. Integer overflow
int a = 2000000000, b = 2000000000;
int c = a + b; // ← overflow! Use long long

// 6. Return without value in non-void function
int find(...) { if (...) return idx; } // ← missing return -1 at end
```

**Conditional breakpoint usage:** If a bug only appears on the 1000th iteration of a loop, a normal breakpoint would require 999 manual "continue" presses. A conditional breakpoint fires only when `i == 999` — invaluable.

---

### 🧩 Practice Tasks

**Easy:** Deliberately introduce the 6 common bugs listed above one at a time into a small program. Practice finding each using the Visual Studio debugger rather than reading the code.

**Medium:** Write a recursive function with a subtle off-by-one bug. Use the call stack window to trace the recursive calls and identify where the invariant breaks.

**Hard:** Write a multi-function program where a null-pointer dereference occurs 5 function calls deep. Practice using the call stack window to navigate up the call chain and identify which function passed a null pointer.

---

# Comprehensive Practice Task Bank

> A unified collection of problems organised by topic and difficulty. Each builds on concepts from multiple lectures.

---

## Topic 1: Classes & Pointers

**Easy**

- Create a `Circle` class with `radius`, `area()`, and `circumference()`.
- Write `swap(int*, int*)` using raw pointers.

**Medium**

- Implement a `BankAccount` class with private `balance`, validated `deposit()`/`withdraw()`, and a getter. Prevent overdrafts.
- Write a generic `ObjectPool<T, N>` that pre-allocates N objects and dispenses/reclaims without `new`/`delete` in using code.

**Hard**

- Implement the Rule of Three (copy constructor, copy-assignment, destructor) for a `Matrix` class that heap-allocates a 2D array.
- Design a smart pointer class `MyUniquePtr<T>` that mimics `std::unique_ptr`: move-only, auto-deletes on destruction.

---

## Topic 2: Stacks

**Easy**

- Reverse a string using a stack.
- Check if parentheses are balanced: `{[()]}` → true, `{[(])}` → false.

**Medium**

- Evaluate a postfix expression: `"6 5 2 3 + 8 * + 3 + *"` → 288.
- String editor: `#` = backspace, `$` = repeat last char. `"ac$db#"` → `"accd"`.

**Hard**

- Implement `MinStack`: `push`, `pop`, `top`, and `getMin` all in O(1).
- Convert infix expression to postfix using the shunting-yard algorithm, handling operator precedence and parentheses.

---

## Topic 3: Queues

**Easy**

- Card deck problem: n cards, repeatedly discard top and move new top to bottom. Find discarded order (n=10 → 1,3,5,7,9,2,6,10,8; remaining: 4).
- Implement `replace(Q, v1, v2)` using only queue operations.

**Medium**

- Implement a queue using two stacks (amortised O(1) per operation).
- Implement a circular buffer of size N that overwrites oldest data when full.

**Hard**

- Given a stream of data packets (each with a priority and timestamp), design a system that serves highest-priority packets first, but within the same priority serves in FIFO order. Use STL containers.

---

## Topic 4: Array Lists & Complexity

**Easy**

- Determine Big-O for: nested loop `for i in 1..n: for j in i..n`, binary search, single loop with constant-time body.
- Given a sorted vector, find a target using binary search.

**Medium**

- Find two elements summing to a target: O(n²) brute force first, then O(n) with hash set.
- Given a sorted rotated array `[4,5,6,7,0,1,2]`, find a target in O(log N).

**Hard**

- Implement a `DynamicArray<T>` from scratch with `push_back`, `pop_back`, `at`, `insert`, `erase`, and automatic resizing (doubling). Match `std::vector` semantics.

---

## Topic 5: Linked Lists

**Easy**

- Print a linked list in reverse using recursion (no modification).
- Find the middle element of a linked list in one pass (slow/fast pointer).

**Medium**

- Detect a cycle in a linked list (Floyd's algorithm): O(1) space.
- Reverse a linked list in-place in O(N) time, O(1) space.

**Hard**

- Reverse a linked list in groups of k. Last group reversed only if it has k elements.
- Merge k sorted linked lists into one sorted list in O(N log k) using a min-heap.

---

## Topic 6: Binary Search Trees

**Easy**

- Insert values and then print the BST in-order to verify sorted output.
- Find minimum and maximum values in a BST.

**Medium**

- Write `isBST(node, min, max)` that correctly validates the BST property for the entire tree (not just immediate children).
- Find the lowest common ancestor (LCA) of two nodes using the BST property.

**Hard**

- Build a balanced BST from a sorted array in O(N).
- Augment BST nodes with subtree sizes. Implement `rank(val)` and `select(k)` in O(log N).

**Competitive**

- **Kth smallest element** in a BST in O(log N + k) without extra storage.
- **Median of data stream:** Use two heaps to maintain median in O(log N) per insertion, O(1) per query.

---

## Topic 7: Hash Tables

**Easy**

- Count character frequencies in a string using `unordered_map`.
- Find the first non-repeating character in a string.

**Medium**

- Two-sum in O(n): given array and target, find all pairs summing to target using a hash set.
- Group anagrams together from a list of strings using `unordered_map<string, vector<string>>`.

**Hard**

- Implement LRU Cache with O(1) `get` and `put` using `unordered_map` + doubly linked list.
- Longest substring without repeating characters in O(N) using sliding window + hash map.

---

## Topic 8: Graphs

**Easy**

- Build a graph from adjacency list input. Print all vertices and edges.
- Run DFS and BFS from a given source vertex.

**Medium**

- Find all connected components using DFS.
- Detect a cycle in an undirected graph using DFS with a parent pointer.

**Hard**

- Topological sort of a DAG (directed acyclic graph) using DFS with a finished-order stack or Kahn's BFS algorithm.
- Number of Islands (LeetCode #200): 2D grid DFS/BFS.

**Competitive**

- **Dijkstra** from scratch with priority queue. Recover the actual path.
- **Bellman-Ford** for graphs with negative weights. Detect negative cycles.

---

## Topic 9: Priority Queues / Heaps

**Easy**

- Use `priority_queue` to sort an array.
- Find the k largest elements in an array using a min-heap of size k in O(N log k).

**Medium**

- Maintain the running median of a stream using two heaps.
- Implement heap sort in-place.

**Hard**

- Merge k sorted arrays in O(N log k) using a min-heap (where N = total elements).
- Implement Dijkstra's algorithm using a min-heap. Verify correctness on a weighted graph.

---

## Cross-Topic Challenges

**The full course challenge — Hard:** Implement a **simple in-memory key-value store** with the following features:

1. `put(key, value)` and `get(key)` in O(1) average → hash table
2. `getMin()` and `getMax()` in O(log N) → BST or heap
3. `printSorted()` in O(N) → BST in-order
4. `undo()` that reverts the last N operations → stack

Use separate data structures working together. This integrates concepts from almost every lecture.

---

_Document covers Lectures 6–11. Combined with Part 1 (Lectures 1–5), this forms the complete course learning resource._ _Textbook reference: "Data Structures and Algorithm Analysis in C++" — Mark Allen Weiss, 4th Edition._ _Chapters referenced: 4 (BSTs), 5 (Hash Tables), 6 (Priority Queues/Heaps), 9 (Graphs)._