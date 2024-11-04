#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <string>
#include "time_manager.h"
#include <Windows.h>

std::string get_timezone_offset() {
    TIME_ZONE_INFORMATION tz_info;
    DWORD result = GetTimeZoneInformation(&tz_info);

    if (result == TIME_ZONE_ID_INVALID) {
        return "UTC+0"; // 默认返回 UTC+0
    }

    // 计算时区偏移，单位为分钟
    int offset_minutes = tz_info.Bias + (result == TIME_ZONE_ID_DAYLIGHT ? tz_info.DaylightBias : 0);

    // 将偏移转换为小时和分钟，注意offset_hours*-1是因为Windows好像会把和UTF时间的差值与UTF标号正负弄反
    int offset_hours = (-1) * offset_minutes / 60;


    // 构造返回的字符串
    std::string offset_str = "UTC";

    // 根据偏移的正负添加相应的符号
    if (offset_hours > 0) {
        offset_str += "+" + std::to_string(offset_hours);
    }
    else if (offset_hours < 0) {
        offset_str += std::to_string(offset_hours);
    }
    else {
        offset_str += "+0"; // 如果没有偏移，返回 UTC+0
    }

    return offset_str;
}



int main() {
    const std::string filename = "RefTime.json";
    TimeManager manager = TimeManager(filename);
    std::string timezone = get_timezone_offset(); // 目前只支持Windows

    // 显示当前游戏时间
    manager.displayCurrentGameTime(timezone);

    return 0;
}
