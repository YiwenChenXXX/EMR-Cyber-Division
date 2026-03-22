//
// Created by wilso on 3/21/2026.
//
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
    Report* current = head;
    Report* maxNode = head;
    Report* prevMax = nullptr;

    //Find the max
    while (current != nullptr) {
        if (current->severity > maxNode->severity) {
            maxNode = current;
        }
        prevMax = current;
        current = current->sibling;
    }

    //Unlink the max from the list
    if (maxNode != head)
    {
     prevMax->sibling = maxNode->sibling;
    }
    



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
            // attach current
            if (head == nullptr) {
                head = current;
                tail = current;
            } else {
                tail->sibling = current;
                tail = current;
            }

            current = current->sibling;
        } else {
            //attach next
            if (head == nullptr) {
                head = next;
                tail = next;
            } else {
                tail->sibling = next;
                tail = current;
            }

            next = next->sibling;
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