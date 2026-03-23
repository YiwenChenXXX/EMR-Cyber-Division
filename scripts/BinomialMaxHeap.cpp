
#include "BinomialMaxHeap.h"

using namespace std;


void BinomialMaxHeap::insert(int severity) {

    Report* newNode = new Report;
    newNode->severity = severity;

    BinomialMaxHeap tempHeap;
    tempHeap.head = newNode;

    head = mergeHeaps(head, tempHeap.head);
    unionHeaps();
}

Report* BinomialMaxHeap::findMax() {
    if (head == nullptr){
        return nullptr;
    }
    Report* current = head;
    Report* maxNode = head;

    while (current != nullptr) {
        if (current->severity > maxNode->severity) {
            maxNode = current;
        }
        current = current->sibling;
    }

    return maxNode;
}

Report* BinomialMaxHeap::extractMax()
{
    if (head == nullptr){
        return nullptr;
    }
    Report* current = head;
    Report* maxNode = head;
    Report* prevMax = nullptr;
    Report* prev = nullptr;

    //Find the max
    while (current != nullptr) {
        if (current->severity > maxNode->severity) {
            maxNode = current;
            prevMax = prev;   // THIS is the key line
        }

        prev = current;
        current = current->sibling;
    }

    //Unlink the max from the list
    if (maxNode == head) {
        head = maxNode->sibling;
    } else {
        prevMax->sibling = maxNode->sibling;
    }

    //Reverse list
    current = maxNode->child;
    Report* next;
    prev = nullptr;

    while (current != nullptr)
    {
        next = current->sibling;

        current->parent = nullptr;
        current->sibling = prev;

        prev = current;
        current = next;
    }

    maxNode->child = nullptr;

    head = mergeHeaps(head, prev);
    unionHeaps();



    return maxNode;
}

Report* BinomialMaxHeap::mergeHeaps(Report* current, Report* next)
{
    if (current == nullptr) return next;
    if (next == nullptr) return current;

    Report* head = nullptr;
 Report* tail = nullptr;
    while (current != nullptr && next != nullptr)
    {
        if (current->degree <= next->degree) {

            Report* temp = current;           // save node
            current = current->sibling;       // move forward
            temp->sibling = nullptr;          // detach

            if (head == nullptr) {
                head = temp;
                tail = temp;
            } else {
                tail->sibling = temp;         // attach
                tail = temp;
            }

        } else {

            Report* temp = next;              // save node
            next = next->sibling;             // move forward
            temp->sibling = nullptr;          // detach

            if (head == nullptr) {
                head = temp;
                tail = temp;
            } else {
                tail->sibling = temp;         // attach
                tail = temp;
            }
        }
    }
    if (tail == nullptr)
    {
        if (current != nullptr)
        {
            return current;
        } else
        {
            return next;
        }
    }

    if (current != nullptr)
    {
        tail->sibling = current;
    }else
    {
        tail->sibling = next;
    }

    return head;
}

void BinomialMaxHeap::unionHeaps() {
    if (head == nullptr)
    {
        return;
    }
    Report* current = head;
    Report* prev = nullptr;
    Report* next = current->sibling;
    while (next != nullptr){
        if (current->degree != next->degree) {
            prev = current;
            current = next;
        }
        else if (next->sibling != nullptr && next->sibling->degree == current->degree) {
            //handle 3 in a row case
            prev = current;
            current = next;
            next = next->sibling;
        }
        else {
            if (current->severity >= next->severity) {
                // current stays root
                current->sibling = next->sibling;
                linkTrees(next, current);
            }
            else {
                // next becomes root
                if (prev == nullptr)
                    head = next;
                else
                    prev->sibling = next;

                linkTrees(current, next);
                current = next;
            }
        }
        next = current->sibling;
    }

}
