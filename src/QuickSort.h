#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include "DayList.h"

namespace Sorter {
    class QuickSort {
    public:
        // sorts the link list where the head or tail pointers are passed into
        // headPointer points to the earliest day
        // tailPointer points to the last day
        static void sortDays(DayList::Day** headRef, DayList::Day** tailRef, DayList::SortMode mode);

    private:
        static DayList::SortMode s_mode;

        // compares two, day pointers (yy, mm, dd)
        static bool comesBefore(const DayList::Day* a, const DayList::Day* b);

        // moves elements greater than pivot to the left of index
        static int partition(std::vector<DayList::Day*>& arr, int low, int high);

        // quick sort recursion
        static void quickSortVec(std::vector<DayList::Day*>& arr, int low, int high);

    };
}

#endif // QUICKSORT_H