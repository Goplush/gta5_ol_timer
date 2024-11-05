#include "time_manager.h"
#include "json_handler.h"



void TimeManager::readTimeMappings(const std::string& filename) {
    this->mapping = JsonHandler::readMappings(filename);
    return;
}

TimeManager::TimeManager(const std::string& filename) :mapping(JsonHandler::readMappings(filename)) {
    mapping.real_time = TimeManager::Convert2UTC(mapping.real_time);
}

//暂时没用
void TimeManager::saveTimeMapping(const std::string& filename, const TimeMapping& mapping) {
    JsonHandler::saveMapping(filename, mapping);
}

GameTime TimeManager::calculateGameTime(RealTime& sys_time) {
    sys_time = TimeManager::Convert2UTC(sys_time);

    tm tm_real_time = {};
    tm_real_time.tm_year = sys_time.year - 1900; // 年份要减去1900
    tm_real_time.tm_mon = sys_time.month - 1;    // 月份是从0开始的
    tm_real_time.tm_mday = sys_time.day;
    tm_real_time.tm_hour = sys_time.hour;
    tm_real_time.tm_min = sys_time.minute;
    tm_real_time.tm_sec = sys_time.second;

    // 基准时间
    tm tm_base_time = {};
    tm_base_time.tm_year = mapping.real_time.year - 1900;
    tm_base_time.tm_mon = mapping.real_time.month - 1;
    tm_base_time.tm_mday = mapping.real_time.day;
    tm_base_time.tm_hour = mapping.real_time.hour;
    tm_base_time.tm_min = mapping.real_time.minute;
    tm_base_time.tm_sec = mapping.real_time.second;

    // 转换为time_t类型
    time_t tt_sys_time = mktime(&tm_real_time);
    time_t tt_base_time = mktime(&tm_base_time);

    double elapsed_seconds = difftime(tt_sys_time, tt_base_time);

    // 计算游戏时间
    unsigned long long total_minutes = elapsed_seconds / 2; // 现实时间每两秒对应游戏时间每分钟
    GameTime game_time;
    game_time.week = (total_minutes / (24 * 60) + mapping.game_time.week) % 7 ; // 星期
    total_minutes %= (24 * 60);  //剩余的分钟
    game_time.hour = (total_minutes / 60 + mapping.game_time.hour) % 24; // 游戏小时
    game_time.minute = (total_minutes + mapping.game_time.minute) % 60; // 游戏分钟



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
    // 提取时区偏移
    int offset = 0;
    if (time.timezone.substr(0, 4) == "UTC+") {
        offset = std::stoi(time.timezone.substr(4)); // 获取正偏移
    }
    else if (time.timezone.substr(0, 4) == "UTC-") {
        offset = -std::stoi(time.timezone.substr(4)); // 获取负偏移
    }

    // 进行时间转换
    realtime_utc.hour -= offset; // 计算到 UTC 的小时

    // 处理时间溢出
    if (realtime_utc.hour < 0) {
        realtime_utc.hour += 24;
        realtime_utc.day--;
    }
    else if (realtime_utc.hour >= 24) {
        realtime_utc.hour -= 24;
        realtime_utc.day++;
    }

    // 处理日期溢出
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

// 检查是否为闰年
bool TimeManager::is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 获取每个月的天数
int TimeManager::days_in_month(int year, int month) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
    case 4: case 6: case 9: case 11: return 30;
    case 2: return is_leap_year(year) ? 29 : 28;
    default: return 0; // 不合法的月份
    }
}


void TimeManager::displayCurrentGameTime(const std::string& timezone) {
    while (true) {
        // 获取当前系统时间
        time_t now = time(0);
        struct tm* local = localtime(&now);

        // 将系统时间转化为RealTime结构体
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

        // 输出当前游戏时间
        std::cout << "Current Game Time: Week " << game_time.week
            << " " << game_time.hour << ":" << (game_time.minute < 10 ? "0" : "") << game_time.minute
            << "\r"; // \r 是为了在同一行刷新输出

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
