#include "auth.h"
#include <openssl/md5.h>
#include <cstdlib>
#include <ctime>

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
