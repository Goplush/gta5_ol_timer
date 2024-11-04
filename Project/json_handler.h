#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <string>
#include <vector>
#include "time_manager.h"

class JsonHandler {
public:
    static TimeMapping readMappings(const std::string& filename);
    static void saveMapping(const std::string& filename, const TimeMapping& mapping);
};

#endif // JSON_HANDLER_H
