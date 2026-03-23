

#ifndef COP3530_PROJECT2_BINOMIALMAXHEAP_H
#define COP3530_PROJECT2_BINOMIALMAXHEAP_H

#endif //COP3530_PROJECT2_BINOMIALMAXHEAP_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "data_pipeline/data_common.h"

using namespace std;

class BinomialMaxHeap {
private:
    Report* head;
public:
    BinomialMaxHeap() {
        head = nullptr;
    };
    void insert(int severity);
    Report* mergeHeaps(Report* h1, Report* h2);
    void unionHeaps();
    Report* extractMax();
    Report* findMax();


    void linkTrees(Report* a, Report* b) {
        if (a->severity > b->severity)
        {
            std::swap(a, b);
        }


        a->parent = b;
        a->sibling = b->child;
        b->child = a;
        b->degree++;
    }
};
