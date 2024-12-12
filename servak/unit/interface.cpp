#include <UnitTest++.h>
#include "interface.h"
#include <sstream>
#include <iostream>
#include <cstdlib>

// Подмена стандартного вывода для проверки вывода на экран
class OutputRedirect {
public:
    OutputRedirect() {
        old = std::cout.rdbuf(buffer.rdbuf());
    }

    ~OutputRedirect() {
        std::cout.rdbuf(old);
    }

    std::string str() const {
        return buffer.str();
    }

private:
    std::streambuf* old;
    std::ostringstream buffer;
};

// Тест для проверки вывода справки
TEST(CommandLineInterfacePrintHelp) {
    OutputRedirect output;
    CommandLineInterface cli;
    cli.printHelp();
    
    std::string expectedOutput = 
        "Справка по программе:\n"
        "Использование: ./vcalc_server [опции]\n"
        "\nОпции:\n"
        "  -h, --help       Показать эту справку\n"
        "  -v, --version    Показать версию программы\n"
        "  -p, --port PORT  Указать порт для сервера (по умолчанию 33333)\n";
    
    CHECK_EQUAL(expectedOutput, output.str());
}

// Тест для проверки обработки аргументов с флагом --help
TEST(CommandLineInterfaceHandleHelpArgument) {
    OutputRedirect output;
    CommandLineInterface cli;

    // Убедимся, что программа завершится с кодом 0
    CHECK_EQUAL(0, system("./vcalc_server -h"));
    
    // Проверяем вывод
    std::string expectedOutput = 
        "Справка по программе:\n"
        "Использование: ./vcalc_server [опции]\n"
        "\nОпции:\n"
        "  -h, --help       Показать эту справку\n"
        "  -v, --version    Показать версию программы\n"
        "  -p, --port PORT  Указать порт для сервера (по умолчанию 33333)\n";
    
    CHECK_EQUAL(expectedOutput, output.str());
}

// Тест для проверки обработки аргументов с флагом --version
TEST(CommandLineInterfaceHandleVersionArgument) {
    OutputRedirect output;
    CommandLineInterface cli;

    // Убедимся, что программа завершится с кодом 0
    CHECK_EQUAL(0, system("./vcalc_server -v"));
    
    // Проверяем вывод
    CHECK_EQUAL("Версия программы: 1.0.0\n", output.str());
}

// Тест для проверки обработки аргументов с флагом --port
TEST(CommandLineInterfaceHandlePortArgument) {
    OutputRedirect output;
    
    // Запускаем тестовую программу с параметром порта
    CHECK_EQUAL(0, system("./vcalc_server -p 44444"));

    // Проверяем вывод
    CHECK_EQUAL("Запуск сервера на порту: 44444\n", output.str());
}

// Тест для проверки обработки неизвестной опции
TEST(CommandLineInterfaceHandleUnknownOption) {
    OutputRedirect output;

    // Убедимся, что программа завершится с кодом 1
    CHECK_EQUAL(1, system("./vcalc_server -unknown"));

    // Проверяем вывод на наличие сообщения об ошибке
    CHECK(output.str().find("Неизвестная опция: -unknown") != std::string::npos);
}

// Основная функция для запуска тестов
int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
