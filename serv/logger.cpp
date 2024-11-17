#include "logger.h"
#include <fstream>
#include <ctime>

void logError(const std::string& message, const std::string& logFile) {
    std::ofstream log(logFile, std::ios_base::app);
    time_t now = time(0);
    char* dt = ctime(&now);
    log << dt << " - " << message << "n";
}
