#include "logger.h"
#include <fstream>
#include <iostream>

Logger::Logger(const std::string& logFile) : logFile(logFile) {}

void Logger::log(const std::string& message) {
    std::ofstream out(logFile, std::ios_base::app);
    if (out.is_open()) {
        out << message << std::endl;
    } else {
        std::cerr << "Could not open log file: " << logFile << std::endl;
    }
}
