#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "DayList.h"  // brings in DayList::Day and DayList::SortMode
#include <vector>

namespace HeapSort {

    void heapSort(DayList::Day*& head,
                  DayList::Day*& tail,
                  DayList::SortMode mode);
}

#endif // HEAPSORT_H