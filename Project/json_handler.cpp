#include "json_handler.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

TimeMapping JsonHandler::readMappings(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        TimeMapping timeMapping;

        // 打开文件
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        // 解析JSON
        json j;
        inputFile >> j;

        // 将JSON映射到结构体
        timeMapping.real_time.year = j["real_time"]["year"];
        timeMapping.real_time.month = j["real_time"]["month"];
        timeMapping.real_time.day = j["real_time"]["day"];
        timeMapping.real_time.hour = j["real_time"]["hour"];
        timeMapping.real_time.minute = j["real_time"]["minute"];
        timeMapping.real_time.second = j["real_time"]["second"];
        timeMapping.real_time.timezone = j["real_time"]["timezone"];

        timeMapping.game_time.week = j["game_time"]["week"];
        timeMapping.game_time.hour = j["game_time"]["hour"];
        timeMapping.game_time.minute = j["game_time"]["minute"];

        return timeMapping;
    }
    printf("ERROR OPENING FILE");
    throw new std::exception("ERROR OPENING FILE");
}

void JsonHandler::saveMapping(const std::string& filename, const TimeMapping& mapping) {
    std::ifstream file(filename);
    json j;
    if (file.is_open()) {
        file >> j;
    }

    json new_entry = {
        {"real_time", {
            {"year", mapping.real_time.year},
            {"month", mapping.real_time.month},
            {"day", mapping.real_time.day},
            {"hour", mapping.real_time.hour},
            {"minute", mapping.real_time.minute},
            {"second", mapping.real_time.second},
            {"timezone", mapping.real_time.timezone}
        }},
        {"game_time", {
            {"week", mapping.game_time.week},
            {"hour", mapping.game_time.hour},
            {"minute", mapping.game_time.minute}
        }}
    };

    j["time_mappings"].push_back(new_entry);
    std::ofstream out_file(filename);
    out_file << j.dump(4);
}
