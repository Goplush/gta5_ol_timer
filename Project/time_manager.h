#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
#define _CRT_SECURE_NO_WARNINGS 1


#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include<Windows.h>
#include <ctime>


struct RealTime {
    int year, month, day, hour, minute, second;
    std::string timezone;
};

struct GameTime {
    int week; // 0-6,Sunday-Saturady
    int hour;
    int minute;
};

struct TimeMapping {
    RealTime real_time;
    GameTime game_time;
};

class TimeManager {
public:
    TimeManager(const std::string& filename);
    void readTimeMappings(const std::string& filename);
    void saveTimeMapping(const std::string& filename, const TimeMapping& mapping);
    GameTime calculateGameTime(RealTime& real_time);
    void displayCurrentGameTime(const std::string& timezone);

    static RealTime Convert2UTC(const RealTime& time);
    static bool is_leap_year(int year);
    static int days_in_month(int year, int month);

private:
    TimeMapping mapping;
};

#endif // TIME_MANAGER_H
