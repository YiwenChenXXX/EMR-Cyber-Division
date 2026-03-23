#include <iostream>
#include <vector>
#include <algorithm>
#include "data_common.h"
#include "BinaryMaxHeap.h"
using namespace std;


bool BinaryMaxHeap::highestPriority(const Report& reportA, const Report& reportB) {
    if (reportA.severity > reportB.severity) {
        return true;
    }

    if (reportA.severity == reportB.severity && reportA.timestamp < reportB.timestamp) {
        return true;
    }

    return false;
}

void BinaryMaxHeap::heapifyDown(int parentIndex) {

    int heapSize = heap.size();
    while(true) {
        int leftChildIndex = 2 * parentIndex + 1;
        int rightChildIndex = 2 * parentIndex + 2;
        int largestIndex = parentIndex;

    if (leftChildIndex < heapSize && highestPriority(heap[leftChildIndex], heap[largestIndex])) {
        largestIndex = leftChildIndex;
    }

    if (rightChildIndex < heapSize && highestPriority(heap[rightChildIndex], heap[largestIndex])) {
        largestIndex = rightChildIndex;
    }

    if (largestIndex == parentIndex) {
        break;
    }
    swap(heap[parentIndex], heap[largestIndex]);
    parentIndex = largestIndex;

    }
}


void BinaryMaxHeap::insert(const Report& report) {
    heap.push_back(report);
    int currentIndex = heap.size() - 1;
    int parentIndex = 0;
    while (currentIndex > 0) {
        parentIndex = (currentIndex - 1) / 2;
        if (highestPriority(heap[currentIndex], heap[parentIndex])) {
            swap(heap[currentIndex], heap[parentIndex]);
            currentIndex = parentIndex;
        } else {
            break;
        }

    }

}

Report BinaryMaxHeap::extractMax() {
    Report maxReport = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
    return maxReport;

}

bool BinaryMaxHeap::isEmpty() {
    return heap.empty();
}
