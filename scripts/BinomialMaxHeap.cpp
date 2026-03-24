#include "BinomialMaxHeap.h"

using namespace std;

bool BinomialMaxHeap::higherPriority(const Report& a, const Report& b) {
    if (a.severity != b.severity) return a.severity > b.severity;
    return a.timestamp < b.timestamp;
}

void BinomialMaxHeap::insert(int severity) {
    Report r;
    r.report_id = 0;
    r.source_id = 0;
    r.timestamp = 0;
    r.severity = severity;
    r.category = "medical";
    insert(r);
}

void BinomialMaxHeap::insert(const Report& report) {
    Node* newNode = new Node(report);
    BinomialMaxHeap tempHeap;
    tempHeap.head = newNode;
    head = mergeHeaps(head, tempHeap.head);
    unionHeaps();
}

Report BinomialMaxHeap::findMax() {
    Report dummy{};
    dummy.severity = -1;
    if (head == nullptr){
        return dummy;
    }
    Node* current = head;
    Node* maxNode = head;
    while (current != nullptr) {
        if (higherPriority(current->key, maxNode->key)) {
            maxNode = current;
        }
        current = current->sibling;
    }
    return maxNode->key;
}

Report BinomialMaxHeap::extractMax() {
    Report dummy{};
    dummy.severity = -1;
    if (head == nullptr){
        return dummy;
    }
    Node* current = head;
    Node* maxNode = head;
    Node* prevMax = nullptr;
    Node* prev = nullptr;
    while (current != nullptr) {
        if (higherPriority(current->key, maxNode->key)) {
            maxNode = current;
            prevMax = prev;
        }
        prev = current;
        current = current->sibling;
    }
    // unlink maxNode from root list
    if (maxNode == head) {
        head = maxNode->sibling;
    } 
    else {
        prevMax->sibling = maxNode->sibling;
    }
    // reverse child list
    current = maxNode->child;
    Node* next;
    prev = nullptr;
    while (current != nullptr) {
        next = current->sibling;
        current->parent = nullptr;
        current->sibling = prev;
        prev = current;
        current = next;
    }
    maxNode->child = nullptr;
    head = mergeHeaps(head, prev);
    unionHeaps();
    Report ans = maxNode->key;
    delete maxNode;
    return ans;
}

BinomialMaxHeap::Node* BinomialMaxHeap::mergeHeaps(Node* current, Node* next) {
    if (current == nullptr){
        return next;
    }
    if (next == nullptr){
        return current;
    }
    Node* newHead = nullptr;
    Node* tail = nullptr;
    auto attach = [&](Node* x) {
        if (newHead == nullptr) {
            newHead = x;
            tail = x;
        } 
        else {
            tail->sibling = x;
            tail = x;
        }
    };
    while (current != nullptr && next != nullptr) {
        if (current->degree <= next->degree) {
            Node* temp = current;
            current = current->sibling;
            temp->sibling = nullptr;
            attach(temp);
        } 
        else {
            Node* temp = next;
            next = next->sibling;
            temp->sibling = nullptr;
            attach(temp);
        }
    }
    if (tail == nullptr){
        return (current != nullptr) ? current : next;
    }
    tail->sibling = (current != nullptr) ? current : next;
    return newHead;
}

void BinomialMaxHeap::unionHeaps() {
    if (head == nullptr){
        return;
    }
    Node* current = head;
    Node* prev = nullptr;
    Node* next = current->sibling;
    while (next != nullptr) {
        if (current->degree != next->degree) {
            prev = current;
            current = next;
        } 
        else if (next->sibling != nullptr && next->sibling->degree == current->degree) {
            prev = current;
            current = next;
            next = next->sibling;
        } 
        else {
            if (higherPriority(current->key, next->key)) {
                current->sibling = next->sibling;
                linkTrees(next, current);
            } 
            else {
                if (prev == nullptr){
                    head = next;
                }
                else{
                    prev->sibling = next;
                }
                linkTrees(current, next);
                current = next;
            }
        }
        next = current->sibling;
    }
}
