#include "QuickSort.h"
#include <algorithm>

namespace Sorter {
    // Keep track of the current sort mode
    DayList::SortMode QuickSort::s_mode = DayList::BY_CAPACITY;

    // Full comparator that breaks all ties: capacity, weather, then date, AM/PM, and park name
    bool QuickSort::comesBefore(const DayList::Day* a, const DayList::Day* b) {
        switch (s_mode) {
            case DayList::BY_CAPACITY:
                if (a->capacity != b->capacity)
                    return a->capacity < b->capacity;
                if (a->weather != b->weather)
                    return a->weather < b->weather;
                break;

            case DayList::BY_WEATHER:
                if (a->weather != b->weather)
                    return a->weather < b->weather;
                if (a->capacity != b->capacity)
                    return a->capacity < b->capacity;
                break;

            case DayList::BY_BOTH:
                if (a->capacity != b->capacity)
                    return a->capacity < b->capacity;
                if (a->weather != b->weather)
                    return a->weather < b->weather;
                break;
        }
        // Now tie-break completely by date and other fields
        if (a->year != b->year)
            return a->year < b->year;
        if (a->month != b->month)
            return a->month < b->month;
        if (a->day != b->day)
            return a->day < b->day;
        if (a->isBeforeNoon != b->isBeforeNoon)
            return a->isBeforeNoon;  // AM before PM
        return a->park < b->park;    // finally by park name alphabetically
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

    void QuickSort::sortDays(DayList::Day** headRef,
                             DayList::Day** tailRef,
                             DayList::SortMode mode) {
        s_mode = mode;
        std::vector<DayList::Day*> arr;
        for (auto* cur = *headRef; cur; cur = cur->next) {
            arr.push_back(cur);
        }
        if (arr.empty()) return;

        quickSortVec(arr, 0, static_cast<int>(arr.size()) - 1);

        // rebuild linked list
        for (size_t i = 1; i < arr.size(); ++i) {
            arr[i - 1]->next = arr[i];
        }
        arr.back()->next = nullptr;
        *headRef = arr.front();
        *tailRef = arr.back();
    }
}