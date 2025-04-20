#include "QuickSort.h"
#include <algorithm>

namespace Sorter {

    bool QuickSort::comesBefore(const DayList::Day* a, const DayList::Day* b) {
        if (a->year  != b->year)  return a->year  < b->year;
        if (a->month != b->month) return a->month < b->month;

        return a->day   < b->day;
    }

    int QuickSort::partition(std::vector<DayList::Day*>& arr, int low, int high) {
        DayList::Day* pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            if (comesBefore(arr[j], pivot)) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[i+1], arr[high]);
        return i+1;
    }

    void QuickSort::quickSortVec(std::vector<DayList::Day*>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortVec(arr, low, pi - 1);
            quickSortVec(arr, pi + 1, high);
        }
    }

    void QuickSort::sortDays(DayList::Day** headRef, DayList::Day** tailRef) {
        std::vector<DayList::Day*> arr;

        for (DayList::Day* cur = *headRef; cur; cur = cur->next) {
            arr.push_back(cur);
        }

        if (arr.empty()) return;

        // sort
        quickSortVec(arr, 0, static_cast<int>(arr.size()) - 1);

        // rebuild pointers
        for (size_t i = 0; i + 1 < arr.size(); ++i) {
            arr[i]->next = arr[i+1];
        }

        arr.back()->next = nullptr;

        // reset head & tail
        *headRef = arr.front();
        *tailRef = arr.back();
    }

}