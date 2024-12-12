#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include "calc.h"

// Функция для генерации соли длиной 64 бита в шестнадцатеричном формате
std::string generate_salt() {
    unsigned char salt[8]; // 64 бита = 8 байт
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw std::runtime_error("Error generating random bytes for salt");
    }

    // Преобразуем байты в шестнадцатеричный формат
    std::ostringstream oss;
    for (size_t i = 0; i < sizeof(salt); ++i) { // Изменено на size_t
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(salt[i]);
    }
    return oss.str();
}

std::string hash_password(const std::string& password, const std::string& salt) {
    std::string salted_password = password + salt;
    
    unsigned char hash[MD5_DIGEST_LENGTH];
    
    // Явное приведение типов для устранения предупреждения
    MD5(reinterpret_cast<const unsigned char*>(salted_password.c_str()), 
        static_cast<size_t>(salted_password.size()), hash);
    
    // Преобразуем хеш в строку
    std::string hashed_password;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        char buffer[3];
        sprintf(buffer, "%02x", hash[i]);
        hashed_password += buffer;
    }

    return hashed_password;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Set socket options error" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(33333);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
    std::cerr << "Listen failed" << std::endl;
    return -1;
}

std::cout << "Server started on 127.0.0.1:33333" << std::endl;

while (true) {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                             (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        continue;
    }

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    std::string client_message(buffer);

    // Генерация соли
    std::string salt = generate_salt();

    // Отправляем соль клиенту
    send(new_socket, salt.c_str(), salt.length(), 0);

    // Чтение пароля от клиента
    memset(buffer, 0, sizeof(buffer));
    read(new_socket, buffer, 1024);
    std::string password_from_client(buffer);

    // Хешируем пароль с солью
    std::string hashed_password = hash_password(password_from_client, salt);

    // Отправляем ответ клиенту о результате авторизации
    std::string response = "OK"; // Здесь можно добавить проверку пароля
    send(new_socket, response.c_str(), response.length(), 0);

    // Новый код для обработки векторов
    int num_vectors;
    read(new_socket, &num_vectors, sizeof(num_vectors)); // Читаем количество векторов

    for (int i = 0; i < num_vectors; ++i) {
        int vector_size;
        read(new_socket, &vector_size, sizeof(vector_size)); // Читаем размер вектора

        std::vector<double> vec(vector_size);
        read(new_socket, vec.data(), vector_size * sizeof(double)); // Читаем вектор

        // Вычисляем среднее арифметическое
        double average = Calculator::calculate_average(vec);

        // Отправляем результат обратно клиенту
        send(new_socket, &average, sizeof(average), 0);
    }

    close(new_socket);
}

close(server_fd);
return 0;
}
