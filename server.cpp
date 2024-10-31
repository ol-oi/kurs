#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <openssl/md5.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>

const int DEFAULT_PORT = 33333;  // Порт по умолчанию
const std::string DEFAULT_USERS_FILE = "/etc/vcalc.conf";  // Файл пользователей по умолчанию
const std::string DEFAULT_LOG_FILE = "/var/log/vcalc.log";  // Файл журнала по умолчанию
const int SALT_LENGTH = 16;  // Длина соли

std::unordered_map<std::string, std::string> users;  // Хранение пользователей и их паролей
std::string logFilePath = DEFAULT_LOG_FILE;  // Путь к файлу журнала

// Функция для загрузки пользователей из файла
void loadUsers(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        logError("Cannot open users file: " + filename);  // Логирование ошибки
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find(':');  // Разделение логина и пароля
        if (pos != std::string::npos) {
            std::string username = line.substr(0, pos);
            std::string password = line.substr(pos + 1);
            users[username] = password;  // Сохранение пользователя в хэш-таблицу
        }
    }
}

// Функция для генерации соли
std::string generateSalt() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 15);
    std::ostringstream oss;
    for (int i = 0; i < SALT_LENGTH; ++i) {
        oss << std::hex << dist(rd);  // Генерация случайной соли в шестнадцатичном формате
    }
    return oss.str();
}

// Функция для хэширования пароля с использованием соли
std::string hashPassword(const std::string& salt, const std::string& password) {
    std::string input = salt + password;  // Конкатенация соли и пароля
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);  // Хэширование пароля
    
    std::ostringstream oss;
    for (unsigned char c : digest) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);  // Преобразование хэша в строку
    }
    return oss.str();
}

// Функция для логирования ошибок
void logError(const std::string& message) {
    std::ofstream logFile(logFilePath, std::ios_base::app);
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    logFile << std::ctime(&now_c) << ": " << message << "n";  // Запись ошибки в файл журнала
}

// Функция для обработки переполнения
int handleOverflow(int result, int bits) {
    if (result < 0) {
        return -1 * (1 << bits);  // Возврат значения при переполнении
    }
    return result;
}

// Функция для обработки клиента
void handleClient(int clientSocket) {
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);  // Получение логина от клиента

    std::string username(buffer);
    
    // Генерация соли и отправка клиенту
    std::string salt = generateSalt();
    send(clientSocket, salt.c_str(), salt.size(), 0);

    // Получение хэша пароля от клиента
    recv(clientSocket, buffer, sizeof(buffer), 0);
    
    // Проверка аутентификации
    if (users.find(username) != users.end()) {
        std::string expectedHash = hashPassword(salt, users[username]);
        if (expectedHash == buffer) {
            // Аутентификация успешна
            send(clientSocket, "OK", 2, 0);
            
            // Обработка векторов чисел
            recv(clientSocket, buffer, sizeof(buffer), 0);
            std::istringstream iss(buffer);
            int num;
            std::vector<int> numbers;
            while (iss >> num) {
                numbers.push_back(num);  // Сохранение чисел в векторе
            }

            // Вычисление среднего арифметического
            int sum = 0;
            for (int n : numbers) {
              sum += n;  // Суммирование чисел
            }

            int average = handleOverflow(sum / numbers.size(), sizeof(int) * 8 - 1);  // Обработка переполнения
            send(clientSocket, reinterpret_cast<char*>(&average), sizeof(average), 0);  // Отправка среднего клиенту
        } else {
            send(clientSocket, "FAIL", 4, 0);  // Неверный пароль
        }
    } else {
        send(clientSocket, "FAIL", 4, 0);  // Пользователь не найден
    }
}

// Функция для вывода справки по использованию программы
void printHelp() {
    std::cout << "Usage: vcalc [options]n"
              << "Options:n"
              << "  -h            Show this help messagen"
              << "  -u <file>     Set the users file (default: /etc/vcalc.conf)n"
              << "  -l <file>     Set the log file (default: /var/log/vcalc.log)n"
              << "  -p <port>     Set the server port (default: 33333)n";
}

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;  // Инициализация порта по умолчанию
    std::string usersFile = DEFAULT_USERS_FILE;  // Инициализация файла пользователей по умолчанию

    int opt;
    while ((opt = getopt(argc, argv, "hu:l:p:")) != -1) {  // Обработка параметров командной строки
        switch (opt) {
            case 'h':
                printHelp();  // Вывод справки
                return 0;
            case 'u':
                usersFile = optarg;  // Установка файла пользователей
                break;
            case 'l':
                logFilePath = optarg;  // Установка файла журнала
                break;
            case 'p':
                port = atoi(optarg);  // Установка порта сервера
                break;
            default:
                printHelp();  // Вывод справки при неверных параметрах
                return 1;
        }
    }

    loadUsers(usersFile);  // Загрузка пользователей из файла

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);  // Создание сокета для сервера
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Привязка к любому адресу
    serverAddr.sin_port = htons(port);  // Привязка к указанному порту

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));  // Привязка сокета к адресу и порту
    listen(serverSocket, 5);  // Прослушивание входящих соединений

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);  // Принятие входящего соединения
        handleClient(clientSocket);  // Обработка клиента
        close(clientSocket);  // Закрытие сокета клиента
    }

    close(serverSocket);  // Закрытие серверного сокета
    return 0;
}
