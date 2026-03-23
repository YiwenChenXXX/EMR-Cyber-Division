#pragma once
#include "data_common.h"
using namespace std;

class BinaryMaxHeap {
    private:
        vector <Report> heap;

        bool highestPriority(const Report& reportA, const Report& reportB);
        void heapifyDown(int parentIndex);

    public:
        void insert(const Report& report);
        Report extractMax();
        bool isEmpty();
};