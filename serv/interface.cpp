#include "interface.h"
#include "calc.h"
#include "errors.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/md5.h>

#define MAX_CLIENTS 10

void printHelp() {
    std::cout << "Usage: server [user_database_file] [log_file] [port]n";
}

std::unordered_map<std::string, std::string> loadUsers(const std::string& filename) {
    std::unordered_map<std::string, std::string> users;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string id = line.substr(0, pos);
            std::string pass = line.substr(pos + 1);
            users[id] = pass;
        }
    }
    return users;
}

// Функция для генерации соли
std::string generateSalt() {
    uint64_t salt = rand(); // Генерация случайного числа
    char buffer[17];
    snprintf(buffer, sizeof(buffer), "%016llX", salt); // Преобразование в шестнадцатеричный формат
    return std::string(buffer);
}

// Функция для хэширования пароля с использованием соли
std::string hashPassword(const std::string& salt, const std::string& password) {
    std::string input = salt + password;
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    char output[33];
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        snprintf(&output[i * 2], 3, "%02x", hash[i]);
    }
    return std::string(output);
}

// Функция для обработки запроса клиента
void handleClient(int clientSocket, const std::unordered_map<std::string, std::string>& users) {
    char buffer[1024];
    
     // Чтение логина от клиента
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string login(buffer);

    // Генерация соли и отправка её клиенту
    std::string salt = generateSalt();
    send(clientSocket, salt.c_str(), salt.size(), 0);

    // Чтение хэша пароля от клиента
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string clientHash(buffer);

    // Проверка аутентификации
    if (users.find(login) != users.end()) {
        std::string correctHash = hashPassword(salt, users.at(login));
        if (clientHash == correctHash) {
            send(clientSocket, "OK", 2, 0); // Успешная аутентификация
            
            // Обработка числовых векторов
            uint32_t numVectors;
            recv(clientSocket, &numVectors, sizeof(numVectors), 0);
            numVectors = ntohl(numVectors); // Преобразование в сетевой порядок

            for (uint32_t i = 0; i < numVectors; ++i) {
                uint32_t vectorSize;
                recv(clientSocket, &vectorSize, sizeof(vectorSize), 0);
                vectorSize = ntohl(vectorSize); // Преобразование в сетевой порядок

                std::vector<int32_t> values(vectorSize);
                recv(clientSocket, values.data(), vectorSize * sizeof(int32_t), 0);

                // Вычисление среднего арифметического
                int result = calculateAverage(values);
                result = htonl(result); // Преобразование в сетевой порядок
                send(clientSocket, &result, sizeof(result), 0);
            }
        } else {
            send(clientSocket, "ERR", 3, 0); // Ошибка аутентификации
        }
    } else {
        send(clientSocket, "ERR", 3, 0); // Ошибка идентификации
    }

    close(clientSocket); // Закрытие соединения с клиентом
}
