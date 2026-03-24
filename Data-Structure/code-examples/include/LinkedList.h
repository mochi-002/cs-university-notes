#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include "Stack.h"

using namespace std;

// ═══════════════════════════════════════════════
//  Node
// ═══════════════════════════════════════════════
struct Node
{
    int data;
    Node *next;
    Node(int d) : data(d), next(nullptr) {}
};

// ═══════════════════════════════════════════════
//  LinkedList
// ═══════════════════════════════════════════════
class LinkedList
{
private:
    Node *head;
    Node *tail;
    int sz;

    // ── helpers ─────────────────────────────────

    Node *reverse_k_group(Node *node, int k)
    {
        if (!node || k == 1)
            return node;

        // check if k nodes remain
        Node *check = node;
        int count = 0;
        while (check && count < k)
        {
            check = check->next;
            count++;
        }
        if (count < k)
            return node;

        // reverse k nodes
        Node *prev = nullptr;
        Node *curr = node;
        for (int i = 0; i < k; i++)
        {
            Node *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        node->next = reverse_k_group(curr, k);
        return prev;
    }

    void fix_tail()
    {
        Node *curr = head;
        while (curr && curr->next)
            curr = curr->next;
        tail = curr;
    }

    void clear_without_delete()
    {
        head = nullptr;
        tail = nullptr;
        sz = 0;
    }

public:
    // ── constructors / destructor ────────────────

    LinkedList() : head(nullptr), tail(nullptr), sz(0) {}

    ~LinkedList()
    {
        Node *curr = head;
        while (curr)
        {
            Node *next = curr->next;
            delete curr;
            curr = next;
        }
    }

    // ── basic operations ─────────────────────────

    void append(int val) // O(1)
    {
        Node *newNode = new Node(val);
        if (!tail)
            head = tail = newNode;
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        sz++;
    }

    void insertAt(int index, int val) // O(n)
    {
        assert(index >= 0 && index <= sz);
        Node *newNode = new Node(val);
        if (index == 0)
        {
            newNode->next = head;
            head = newNode;
            if (!tail)
                tail = newNode;
        }
        else
        {
            Node *prev = head;
            for (int i = 0; i < index - 1; i++)
                prev = prev->next;
            newNode->next = prev->next;
            prev->next = newNode;
            if (!newNode->next)
                tail = newNode;
        }
        sz++;
    }

    void deleteAt(int index) // O(n)
    {
        assert(index >= 0 && index < sz);
        Node *toDelete;
        if (index == 0)
        {
            toDelete = head;
            head = head->next;
            if (!head)
                tail = nullptr;
        }
        else
        {
            Node *prev = head;
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

    int at(int index) const // O(n)
    {
        assert(index >= 0 && index < sz);
        Node *curr = head;
        for (int i = 0; i < index; i++)
            curr = curr->next;
        return curr->data;
    }

    int length() const { return sz; }
    bool empty() const { return sz == 0; }

    // ── algorithms ───────────────────────────────

    void reverse() // O(n)
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

    void reverse_k_group(int k) // O(n)
    {
        head = reverse_k_group(head, k);
        fix_tail();
    }

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

    // ── print ────────────────────────────────────

    void print() const
    {
        Node *curr = head;
        while (curr)
        {
            cout << curr->data;
            if (curr->next)
                cout << " → ";
            curr = curr->next;
        }
        cout << " → NULL\n";
    }

    void print_reversed() const
    {
        Stack<int> stk;
        Node *curr = head;
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
        cout << " → NULL\n";
    }
};