#include <UnitTest++.h>
#include <string>
#include "server.h"
#include "logger.h"

// Заглушка для класса Logger
class MockLogger : public Logger {
public:
    void log(const std::string& message) override {
        loggedMessages.push_back(message);
    }

    const std::vector<std::string>& getLoggedMessages() const {
        return loggedMessages;
    }

private:
    std::vector<std::string> loggedMessages;
};

// Тест для конструктора Server
TEST(ServerConstructorTest) {
    MockLogger logger;
    Server server("config.cfg", 8080, logger);

    // Проверяем, что объект создан с правильными параметрами
    CHECK_EQUAL("config.cfg", server.configFile); // Необходимо сделать поле public или добавить метод доступа
    CHECK_EQUAL(8080, server.port);
}

// Тест для метода start (предполагая, что он записывает сообщение в лог)
TEST(ServerStartTest) {
    MockLogger logger;
    Server server("config.cfg", 8080, logger);

    // Вызываем метод start
    server.start();

    // Проверяем, что логгер записал сообщение о запуске сервера
    const auto& messages = logger.getLoggedMessages();
    CHECK(!messages.empty());
    CHECK_EQUAL("Server started on port 8080", messages.back()); // Предполагаем, что это сообщение будет в логе
}

// Основная функция для запуска тестов
int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
