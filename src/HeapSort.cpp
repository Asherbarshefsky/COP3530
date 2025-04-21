#include "HeapSort.h"
#include <algorithm>

// Compare two, Day pointers based on the selected mode
static bool compareDays(const DayList::Day* a,
                        const DayList::Day* b,
                        DayList::SortMode mode) {
    // 1) primary compare by mode
    if (mode == DayList::BY_CAPACITY) {
        if (a->capacity != b->capacity)
            return a->capacity < b->capacity;
        if (a->weather  != b->weather)
            return a->weather < b->weather;
    }
    else if (mode == DayList::BY_WEATHER) {
        if (a->weather  != b->weather)
            return a->weather < b->weather;
        if (a->capacity != b->capacity)
            return a->capacity < b->capacity;
    }
    else {
        if (a->capacity != b->capacity)
            return a->capacity < b->capacity;
        if (a->weather  != b->weather)
            return a->weather < b->weather;
    }

    // 2) now tie‑break fully by date, AM/PM, then park name
    if (a->year         != b->year)          return a->year         < b->year;
    if (a->month        != b->month)         return a->month        < b->month;
    if (a->day          != b->day)           return a->day          < b->day;
    if (a->isBeforeNoon != b->isBeforeNoon)  return a->isBeforeNoon;       // AM (true) before PM (false)
    return a->park < b->park;                                         // finally alphabetical
}

// Sift-down within arr[start..end] to maintain max-heap
static void siftDown(std::vector<DayList::Day*>& arr,
                     int start, int end,
                     DayList::SortMode mode) {
    int root = start;
    while (true) {
        int child = 2 * root + 1;
        if (child > end) break;

        int swapIdx = root;
        if (compareDays(arr[swapIdx], arr[child], mode))
            swapIdx = child;
        if (child + 1 <= end && compareDays(arr[swapIdx], arr[child + 1], mode))
            swapIdx = child + 1;
        if (swapIdx == root) return;

        std::swap(arr[root], arr[swapIdx]);
        root = swapIdx;
    }
}

namespace HeapSort {

void heapSort(DayList::Day*& head,
              DayList::Day*& tail,
              DayList::SortMode mode) {
    // 1. Collect nodes into vector
    std::vector<DayList::Day*> nodes;
    for (auto* cur = head; cur; cur = cur->next) {
        nodes.push_back(cur);
    }

    int n = static_cast<int>(nodes.size());
    if (n < 2) return;

    // 2. Build max-heap
    for (int start = (n - 2) / 2; start >= 0; --start) {
        siftDown(nodes, start, n - 1, mode);
    }

    // 3. Heap sort
    for (int end = n - 1; end > 0; --end) {
        std::swap(nodes[0], nodes[end]);
        siftDown(nodes, 0, end - 1, mode);
    }

    // 4. Relink into single linked list
    head = nodes[0];
    for (int i = 1; i < n; ++i) {
        nodes[i - 1]->next = nodes[i];
    }
    nodes[n - 1]->next = nullptr;
    tail = nodes[n - 1];
}

}