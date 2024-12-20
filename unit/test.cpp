#include <UnitTest++/UnitTest++.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "../../servak/source/headers/server.cpp"
#include "../../servak/source/headers/calc.cpp"
#include "../../servak/source/headers/server.h"
#include "../../servak/source/headers/calc.h"

// Тест для генерации соли
TEST(GenerateSaltTest) {
    std::string salt = generate_salt();
    CHECK_EQUAL(salt.length(), 16); // Должно быть 64 бита, т.е. 16 символов в шестнадцатеричном формате
}

// Тест для хеширования пароля
TEST(HashPasswordTest) {
    std::string password = "P@ssword";
    std::string salt = "12345678"; // Пример соли
    std::string hashed_password = hash_password(password, salt);
    
    // Ожидаемое значение хеша (вычислено заранее)
    std::string expected_hash = "a7b4f4a4f2c7c1d0b8b2f7e3b5b6a0d8"; // Пример ожидаемого значения
    
    CHECK_EQUAL(hashed_password, expected_hash);
}

// Тест для корректности работы сокета (можно использовать мок или заглушку)
TEST(SocketCreationTest) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    
    CHECK((server_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0);
    CHECK(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(33333);
    
    CHECK(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == 0);
    CHECK(listen(server_fd, 3) == 0);
    
    close(server_fd); // Закрываем сокет после теста
}
TEST(LoggerTests, LogMessageSuccessfully) {
    Logger logger(logFileName);
    std::string message = "This is a test log message.";

    // Логируем сообщение
    logger.log(message);

    // Проверяем, что сообщение записано в файл
    std::ifstream in(logFileName);
    std::string line;
    bool messageFound = false;

    if (in.is_open()) {
        while (std::getline(in, line)) {
            if (line == message) {
                messageFound = true;
                break;
            }
        }
    }
    
    CHECK(messageFound);
}

TEST(LoggerTests, LogFileCreation) {
    Logger logger(logFileName);
    
    // Логируем пустое сообщение
    logger.log("");

    // Проверяем, что файл создан
    std::ifstream in(logFileName);
    CHECK(in.is_open());
}

TEST(LoggerTests, LogFileOpenError) {
    // Создаем логгер с недоступным файлом
    Logger logger("/invalid/path/log.txt");

    // Ожидаем, что сообщение об ошибке будет выведено в stderr
    // Для этого перенаправляем stderr во временный файл
    std::string tempFile = "temp_stderr.txt";
    freopen(tempFile.c_str(), "w", stderr);

    logger.log("Test message");

    fclose(stderr);
    
    // Читаем содержимое временного файла
    std::ifstream in(tempFile);
    std::string errorLine;
    bool errorMessageFound = false;

    if (in.is_open()) {
        while (std::getline(in, errorLine)) {
            if (errorLine.find("Could not open log file") != std::string::npos) {
                errorMessageFound = true;
                break;
            }
        }
    }
TEST(AuthTests, LoginWithValidCredentials) {
    CHECK(auth.login("user", "P@ssW0rd"));
}

TEST(AuthTests, LoginWithInvalidUsername) {
    CHECK(!auth.login("invalidUser", "P@ssW0rd"));
}

TEST(AuthTests, LoginWithInvalidPassword) {
    CHECK(!auth.login("user", "wrongPassword"));
}

TEST(AuthTests, LoginWithBothInvalidCredentials) {
    CHECK(!auth.login("invalidUser", "wrongPassword"));
}

TEST(AuthTests, LoginWithEmptyCredentials) {
    CHECK(!auth.login("", ""));
}
TEST(CalculatorTests, AverageOfEmptyVector) {
    std::vector<double> vec;
    CHECK_EQUAL(0.0, calculator.calculate_average(vec));
}

TEST(CalculatorTests, AverageOfSingleElementVector) {
    std::vector<double> vec = {5.0};
    CHECK_EQUAL(5.0, calculator.calculate_average(vec));
}

TEST(CalculatorTests, AverageOfMultipleElementsVector) {
    std::vector<double> vec = {1.0, 2.0, 3.0, 4.0, 5.0};
    CHECK_EQUAL(3.0, calculator.calculate_average(vec));
}

TEST(CalculatorTests, AverageWithNegativeAndPositiveNumbers) {
    std::vector<double> vec = {-1.0, 0.0, 1.0};
    CHECK_EQUAL(0.0, calculator.calculate_average(vec));
}

TEST(CalculatorTests, AverageOfLargeNumbers) {
    std::vector<double> vec = {1000000.0, 2000000.0, 3000000.0};
    CHECK_EQUAL(2000000.0, calculator.calculate_average(vec));
}




// Функция для запуска всех тестов
int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
