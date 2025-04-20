#include <iostream>
#include "DayList.h"

int main() {
    const auto *days = new DayList();

    const DayList::Day *next = days->getHead();

    while (next != nullptr) {
        std::cout << next->park << '\t' << next->month << '/' << next->day << '/' << next->year << '\t' << next->
                capacity << '\n';
        next = next->next;
    }
}
