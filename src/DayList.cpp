#include "DayList.h"
#include "HeapSort.h"
#include "QuickSort.h"
#include <iostream>
#include <cmath>

DayList::DayList() {
    this->head = nullptr;
    this->tail = nullptr;

    this->parkList = {
        "Magic Kingdom",
        "Disneyland",
        "Universal Islands of Adventure",
        "Disney's Hollywood Studios",
        "Universal Studios Florida",
        "Epcot",
        "Disney's Animal Kingdom",
        "Disney California Adventure",
        "Universal Studios Hollywood",
        "SeaWorld Orlando",
        "Busch Gardens Tampa Bay",
        "Knott's Berry Farm",
        "SeaWorld San Diego",
        "Cedar Point",
        "Kings Island"
    }; // In descending order by attendence

    capacityNoise = new PerlinNoise(1 << 6);
    weatherNoise = new PerlinNoise(1 << 2);

    populateDayList();
}

DayList::~DayList() {
    const Day *next = head;

    while (next != nullptr) {
        const Day *day = next;
        next = next->next;
        delete day;
    }
}


int DayList::getMonthLength(const int month, const int year) {
    int monthLength = 31;

    if (month == 2) {
        if (year % 4 == 0)
            monthLength = 29;
        else
            monthLength = 28;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
        monthLength = 30;

    return monthLength;
}

float DayList::getBaseCapacity(const int dayOfWeek, const int parkRank) {
    float base = 0.2f + 0.05f * dayOfWeek;

    if (dayOfWeek == 5)
        base += 0.2;
    else if (dayOfWeek == 6)
        base += 0.3;
    else if (dayOfWeek == 7)
        base += 0.1;

    base += 0.05f / parkRank;

    // Max possible value: 0.85
    return base;
}

bool DayList::checkIsOpen(const int m, const std::string &park) {
    // These parks are closed during the winter months
    if (park == "Cedar Point" || park == "Kings Island")
        if (m > 10 || m < 5)
            return false;

    return true;
}


DayList::Day *DayList::generateDay(const int y, const int m, const int d, const int dayCount, const std::string &park,
                                   const int parkRank) const {
    const int weather = std::round((this->weatherNoise->getNoise(dayCount) + 1) * 2.5) - 1;
    const int dayOfWeek = dayCount % 7 + 1; // Monday = 1, Tuesday = 2, etc.

    const bool isOpen = checkIsOpen(m, park);

    float capacity = 0;
    float afterNoonBias = 0;

    // Only calculate the capacity if the park is open
    if (isOpen) {
        const float noiseValue = this->capacityNoise->getNoise(dayCount) / 10; // Max possible value: 0.1

        const float baseCapacity = getBaseCapacity(dayOfWeek, parkRank); // Max possible value: 0.85

        afterNoonBias = (noiseValue + 0.1) / 4; // Max possible value: 0.05
        capacity = baseCapacity + noiseValue;
    }

    const auto day = new Day(y, m, d, dayOfWeek, true, park, capacity, weather, isOpen);

    capacity += afterNoonBias;

    day->next = new Day(y, m, d, dayOfWeek, false, park, capacity, weather, isOpen);

    return day;
}

void DayList::generateParkStats(const std::string &park, const int parkRank) {
    const int capacitySeed = int(park[0]) * park.length() * (parkRank + 1);
    const int weatherSeed = pow(capacitySeed % static_cast<int>(1e4), 2);

    this->capacityNoise->setSeed(capacitySeed);
    this->weatherNoise->setSeed(weatherSeed);

    int dayCount = 3; // 3 correlates with Thursday, the first day of 2015

    for (int y = 2015; y < 2025; y++) {
        for (int m = 1; m < 13; m++) {
            const int monthLength = getMonthLength(m, y);

            for (int d = 1; d < monthLength + 1; d++) {
                const auto day = generateDay(y, m, d, dayCount, park, parkRank);
                addDay(day);

                dayCount++;
            }
        }
    }
}


void DayList::populateDayList() {
    for (int p = 0; p < this->parkList.size(); p++) {
        generateParkStats(parkList[p], p + 1);
    }
}


void DayList::addDay(Day *day) {
    if (this->head == nullptr) {
        this->head = day;
        this->tail = day;
        return;
    }

    this->tail->next = day;
    this->tail = day->next;
}

void DayList::sort(SortMode mode) {
    HeapSort::heapSort(head, tail, mode);
}

void DayList::sort(SortMode mode, bool useQuick) {
    if (useQuick) {
        Sorter::QuickSort::sortDays(&head, &tail, mode);
    } else {
        HeapSort::heapSort(head, tail, mode);
    }
}

DayList::Day *DayList::getHead() const {
    return this->head;
}