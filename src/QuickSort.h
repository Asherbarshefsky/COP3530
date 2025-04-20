#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include "DayList.h"

namespace Sorter {
    class QuickSort {
    public:
        // sorts the link list where the head or tail pointers are passed into
        static void sortDays(DayList::Day** headRef, DayList::Day** tailRef);

    private:
        // pivot & partition helpers
        static bool comesBefore(const DayList::Day* a, const DayList::Day* b);

        static int partition(std::vector<DayList::Day*>& arr, int low, int high);

        static void quickSortVec(std::vector<DayList::Day*>& arr, int low, int high);
    };
}

#endif // QUICKSORT_H