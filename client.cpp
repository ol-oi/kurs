#include <iostream>
#include <cstring>
#include <openssl/md5.h>
#include <arpa/inet.h>
#include <unistd.h>

// Функция для хэширования пароля с использованием соли
void hashPassword(const std::string& salt, const std::string& password, unsigned char* output) {
    std::string input = salt + password;  // Конкатенация соли и пароля
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), output);  // Хэширование пароля
}

int main(int argc, char* argv[]) {
    const char* serverIp = "127.0.0.1"; // IP сервера (можно заменить на реальный адрес)
    int port = 33333; // Порт сервера по умолчанию

    int sock = socket(AF_INET, SOCK_STREAM, 0);  // Создание сокета для клиента
    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);  
    
    inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);  // Преобразование IP-адреса в формат сети
    
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {  
        perror("Connection failed");  
        return -1;  
    }

    // Ввод логина пользователем
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;

    send(sock, username.c_str(), username.size(), 0);  // Отправка логина на сервер

    // Получение соли от сервера
    char salt[32];

    recv(sock, salt, sizeof(salt), 0);

    // Ввод пароля пользователем
    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;

    // Хэширование пароля с использованием соли
    unsigned char hash[MD5_DIGEST_LENGTH];
    hashPassword(std::string(salt), password, hash);

    send(sock, hash, sizeof(hash), 0);  // Отправка хэша пароля на сервер

    // Проверка результата аутентификации от сервера
    char response[4];
    recv(sock, response, sizeof(response), 0);
    
    if (strncmp(response, "OK", 2) == 0) {  
        // Успешная аутентификация
        std::cout << "Authenticated successfully.n";

        // Ввод чисел пользователем
        std::cout << "Enter numbers separated by space: ";
        std::string numbers;
        getline(std::cin >> std::ws, numbers); // Считывание всей строки

        send(sock, numbers.c_str(), numbers.size(), 0);  // Отправка чисел на сервер

        // Получение среднего арифметического от сервера
        int average;
        recv(sock, &average, sizeof(average), 0);

        if (average < 0) {
            std::cout << "Overflow occurred: " << average << "n";  
        } else {
            std::cout << "Average: " << average << "n";  
        }
        
    } else {  
        std::cout << "Authentication failed.n";  
    }

    close(sock);   // Закрытие сокета клиента
    return 0;  
}
