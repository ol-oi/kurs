#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "logger.h"

class Server {
public:
    Server(const std::string& configFile, int port, Logger& logger);
    void start();

private:
    std::string configFile;
    int port;
    Logger& logger;
};

#endif // SERVER_H
