#include <UnitTest++.h>
#include "logger.h"
#include <fstream>
#include <cstdio> // Для std::remove

// Удаляем файл лога после каждого теста
struct LogFileCleanup {
    LogFileCleanup(const std::string& file) : logFile(file) {}
    ~LogFileCleanup() { std::remove(logFile.c_str()); }
    std::string logFile;
};

// Тест для проверки успешной записи в лог файл
TEST(LoggerLogToFile) {
    const std::string logFile = "test_log.txt";
    LogFileCleanup cleanup(logFile); // Удаляем файл после теста

    Logger logger(logFile);
    logger.log("Test message 1");

    // Проверяем, что сообщение записано в файл
    std::ifstream in(logFile);
    std::string line;
    getline(in, line);
    
    CHECK_EQUAL("Test message 1", line);
}

// Тест для проверки обработки ошибки при открытии файла
TEST(LoggerLogFileOpenError) {
    const std::string logFile = "/invalid/path/test_log.txt"; // Неверный путь
    Logger logger(logFile);

    // Перенаправляем стандартный вывод для проверки ошибок
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    logger.log("This should not be logged.");

    // Восстанавливаем стандартный вывод
    std::cout.rdbuf(oldCout);

    // Проверяем, что вывод содержит сообщение об ошибке
    CHECK(buffer.str().find("Could not open log file") != std::string::npos);
}

// Основная функция для запуска тестов
int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
