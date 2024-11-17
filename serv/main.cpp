#include "interface.h"
#include "auth.h"
#include "logger.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#define DEFAULT_USER_DB "/etc/vcalc.conf"
#define DEFAULT_LOG_FILE "/var/log/vcalc.log"
#define DEFAULT_PORT 33333

int main(int argc, char* argv[]) {
    std::string userDbFile = DEFAULT_USER_DB;
    std::string logFile = DEFAULT_LOG_FILE;
    int port = DEFAULT_PORT;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            printHelp();
            return 0;
        } else if (i == 1) {
            userDbFile = argv[i];
        } else if (i == 2) {
            logFile = argv[i];
        } else if (i == 3) {
            port = atoi(argv[i]);
        }
    }

    srand(time(0)); // Инициализация генератора случайных чисел

    // Загрузка базы пользователей
    auto users = loadUsers(userDbFile);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        logError("Failed to create socket", logFile);
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        logError("Failed to bind socket", logFile);
        return 1;
    }

    listen(serverSocket, MAX_CLIENTS);

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            logError("Failed to accept connection", logFile);
            continue;
        }

        handleClient(clientSocket, users); // Обработка клиента
    }

    close(serverSocket); // Закрытие серверного сокета
    return 0;
}
