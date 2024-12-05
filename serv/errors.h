#ifndef ERRORS_H
#define ERRORS_H

#include <string>

class ErrorLogger {
public:
    static void logError(const std::string& message, const std::string& logFile);
};

#endif // ERRORS_H
