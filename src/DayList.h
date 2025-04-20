#ifndef DAYLIST_H
#define DAYLIST_H

#include "PerlinNoise.h"
#include <string>
#include <utility>
#include <vector>


class DayList {
public:
    struct Day {
        unsigned int year;
        unsigned int month;
        unsigned int day;
        unsigned int dayOfWeek;
        bool isBeforeNoon;
        std::string park;
        float capacity;
        unsigned int weather;
        bool isOpen;
        Day *next{};

        Day(const unsigned int year, const unsigned int month, const unsigned int day, unsigned int dayOfWeek,
            const bool isBeforeNoon, std::string park, const float capacity, const unsigned int weather,
            const bool isOpen): year(year), month(month), day(day),
                                dayOfWeek(dayOfWeek), isBeforeNoon(isBeforeNoon), park(std::move(park)),
                                capacity(capacity),
                                weather(weather),
                                isOpen(isOpen) {
        }
    };

    DayList();

    ~DayList();

    void populateDayList();

    void addDay(Day *day);

    Day *getHead() const;

private:
    Day *head;
    Day *tail;

    std::vector<std::string> parkList;

    PerlinNoise *capacityNoise;
    PerlinNoise *weatherNoise;

    static int getMonthLength(int month, int year);

    static float getBaseCapacity(int dayOfWeek, int parkRank);

    static bool checkIsOpen(int m, const std::string &park);

    Day *generateDay(int y, int m, int d, int dayCount, const std::string &park, int parkRank) const;

    void generateParkStats(const std::string &park, int parkRank);
};


#endif
