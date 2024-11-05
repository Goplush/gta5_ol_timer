#include "time_manager.h"
#include "json_handler.h"



void TimeManager::readTimeMappings(const std::string& filename) {
    this->mapping = JsonHandler::readMappings(filename);
    return;
}

TimeManager::TimeManager(const std::string& filename) :mapping(JsonHandler::readMappings(filename)) {
    mapping.real_time = TimeManager::Convert2UTC(mapping.real_time);
}

//��ʱû��
void TimeManager::saveTimeMapping(const std::string& filename, const TimeMapping& mapping) {
    JsonHandler::saveMapping(filename, mapping);
}

GameTime TimeManager::calculateGameTime(RealTime& sys_time) {
    sys_time = TimeManager::Convert2UTC(sys_time);

    tm tm_real_time = {};
    tm_real_time.tm_year = sys_time.year - 1900; // ���Ҫ��ȥ1900
    tm_real_time.tm_mon = sys_time.month - 1;    // �·��Ǵ�0��ʼ��
    tm_real_time.tm_mday = sys_time.day;
    tm_real_time.tm_hour = sys_time.hour;
    tm_real_time.tm_min = sys_time.minute;
    tm_real_time.tm_sec = sys_time.second;

    // ��׼ʱ��
    tm tm_base_time = {};
    tm_base_time.tm_year = mapping.real_time.year - 1900;
    tm_base_time.tm_mon = mapping.real_time.month - 1;
    tm_base_time.tm_mday = mapping.real_time.day;
    tm_base_time.tm_hour = mapping.real_time.hour;
    tm_base_time.tm_min = mapping.real_time.minute;
    tm_base_time.tm_sec = mapping.real_time.second;

    // ת��Ϊtime_t����
    time_t tt_sys_time = mktime(&tm_real_time);
    time_t tt_base_time = mktime(&tm_base_time);

    double elapsed_seconds = difftime(tt_sys_time, tt_base_time);

    // ������Ϸʱ��
    unsigned long long total_minutes = elapsed_seconds / 2; // ��ʵʱ��ÿ�����Ӧ��Ϸʱ��ÿ����
    GameTime game_time;
    game_time.week = (total_minutes / (24 * 60) + mapping.game_time.week) % 7 ; // ����
    total_minutes %= (24 * 60);  //ʣ��ķ���
    game_time.hour = (total_minutes / 60 + mapping.game_time.hour) % 24; // ��ϷСʱ
    game_time.minute = (total_minutes + mapping.game_time.minute) % 60; // ��Ϸ����



    return game_time;
}

RealTime TimeManager::Convert2UTC(const RealTime& time) {
    RealTime realtime_utc;

    realtime_utc.day = time.day;
    realtime_utc.hour = time.hour;
    realtime_utc.month = time.month;
    realtime_utc.year = time.year;
    realtime_utc.minute = time.minute;
    realtime_utc.second = time.second;;
    realtime_utc.timezone = "UTC+0";
    // ��ȡʱ��ƫ��
    int offset = 0;
    if (time.timezone.substr(0, 4) == "UTC+") {
        offset = std::stoi(time.timezone.substr(4)); // ��ȡ��ƫ��
    }
    else if (time.timezone.substr(0, 4) == "UTC-") {
        offset = -std::stoi(time.timezone.substr(4)); // ��ȡ��ƫ��
    }

    // ����ʱ��ת��
    realtime_utc.hour -= offset; // ���㵽 UTC ��Сʱ

    // ����ʱ�����
    if (realtime_utc.hour < 0) {
        realtime_utc.hour += 24;
        realtime_utc.day--;
    }
    else if (realtime_utc.hour >= 24) {
        realtime_utc.hour -= 24;
        realtime_utc.day++;
    }

    // �����������
    while (realtime_utc.day <= 0) {
        realtime_utc.month--;
        if (realtime_utc.month <= 0) {
            realtime_utc.month = 12;
            realtime_utc.year--;
        }
        realtime_utc.day += days_in_month(realtime_utc.year, realtime_utc.month);
    }

    while (realtime_utc.day > days_in_month(realtime_utc.year, realtime_utc.month)) {
        realtime_utc.day -= days_in_month(realtime_utc.year, realtime_utc.month);
        realtime_utc.month++;
        if (realtime_utc.month > 12) {
            realtime_utc.month = 1;
            realtime_utc.year++;
        }
    }
    return realtime_utc;
}

// ����Ƿ�Ϊ����
bool TimeManager::is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// ��ȡÿ���µ�����
int TimeManager::days_in_month(int year, int month) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
    case 4: case 6: case 9: case 11: return 30;
    case 2: return is_leap_year(year) ? 29 : 28;
    default: return 0; // ���Ϸ����·�
    }
}


void TimeManager::displayCurrentGameTime(const std::string& timezone) {
    while (true) {
        // ��ȡ��ǰϵͳʱ��
        time_t now = time(0);
        struct tm* local = localtime(&now);

        // ��ϵͳʱ��ת��ΪRealTime�ṹ��
        RealTime sys_time = {
            local->tm_year + 1900,
            local->tm_mon + 1,
            local->tm_mday,
            local->tm_hour,
            local->tm_min,
            local->tm_sec,
            timezone
        };

        GameTime game_time = calculateGameTime(sys_time);

        // �����ǰ��Ϸʱ��
        std::cout << "Current Game Time: Week " << game_time.week
            << " " << game_time.hour << ":" << (game_time.minute < 10 ? "0" : "") << game_time.minute
            << "\r"; // \r ��Ϊ����ͬһ��ˢ�����

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
