#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static void logError(const std::string& message, const std::string& logFile);
};

#endif // LOGGER_H
