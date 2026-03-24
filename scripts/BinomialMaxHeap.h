#ifndef COP3530_PROJECT2_BINOMIALMAXHEAP_H
#define COP3530_PROJECT2_BINOMIALMAXHEAP_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "data_pipeline/data_common.h"

using namespace std;

class BinomialMaxHeap {
private:
    struct Node {
        Report key;
        Node* parent;
        Node* child;
        Node* sibling;
        int degree;
        explicit Node(const Report& r):key(r), parent(nullptr), child(nullptr), sibling(nullptr), degree(0) {}
    };
    Node* head;
    static bool higherPriority(const Report& a, const Report& b);

public:
    BinomialMaxHeap(){ 
        head = nullptr; 
    }
    bool isEmpty() const{ 
        return head == nullptr;
    }
    void insert(int severity);
    void insert(const Report& report);
    Node* mergeHeaps(Node* h1, Node* h2);
    void unionHeaps();
    Report extractMax();
    Report findMax();
    void linkTrees(Node* a, Node* b) {
        a->parent = b;
        a->sibling = b->child;
        b->child = a;
        b->degree++;
    }
};

#endif // COP3530_PROJECT2_BINOMIALMAXHEAP_H
