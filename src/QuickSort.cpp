#include "QuickSort.h"
#include <algorithm>

namespace Sorter {

    // Default to capacity sorting
    DayList::SortMode QuickSort::s_mode = DayList::BY_CAPACITY;

    void QuickSort::sortDays(DayList::Day** headRef,
                             DayList::Day** tailRef,
                             DayList::SortMode mode) {
        s_mode = mode;

        // Move list into vector
        std::vector<DayList::Day*> arr;
        for (auto* cur = *headRef; cur; cur = cur->next)
            arr.push_back(cur);
        if (arr.empty()) return;

        // Sort vector
        quickSortVec(arr, 0, static_cast<int>(arr.size()) - 1);

        // Rebuild linked list
        for (size_t i = 0; i + 1 < arr.size(); ++i) {
            arr[i]->next = arr[i + 1];
        }
        arr.back()->next = nullptr;

        // Update head/tail
        *headRef = arr.front();
        *tailRef = arr.back();
    }

    bool QuickSort::comesBefore(const DayList::Day* a, const DayList::Day* b) {
        switch (s_mode) {
            case DayList::BY_CAPACITY:
                return a->capacity < b->capacity;
            case DayList::BY_WEATHER:
                return a->weather < b->weather;
            case DayList::BY_BOTH:
                if (a->capacity != b->capacity)
                    return a->capacity < b->capacity;
                return a->weather < b->weather;
        }
        return false;
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
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void QuickSort::quickSortVec(std::vector<DayList::Day*>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortVec(arr, low, pi - 1);
            quickSortVec(arr, pi + 1, high);
        }
    }

}