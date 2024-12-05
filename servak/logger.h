#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    Logger(const std::string& logFile);
    void log(const std::string& message);

private:
    std::string logFile;
};

#endif // LOGGER_H
