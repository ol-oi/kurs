#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "logger.h"
#include "interface.h"
#include "calc.h"
/*
*@brief Класс server основной цикл для обработки сетевых операций. Запускает сервер
**/
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
