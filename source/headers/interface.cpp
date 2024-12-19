#include "interface.h"
#include <iostream>
#include <cstdlib>

///
/// @brief - Выводит справку
/// @param -h, -v, -p
///
void CommandLineInterface::printHelp() const {
    std::cout << "Справка по программе:\n";
    std::cout << "Использование: ./vcalc_server [опции]\n";
    std::cout << "\nОпции:\n";
    std::cout << "  -h, --help       Показать эту справку\n";
    std::cout << "  -v, --version    Показать версию программы\n";
    std::cout << "  -p, --port PORT  Указать порт для сервера (по умолчанию 33333)\n";
}

///
/// @brief - Отвечает за интерфейс сервера
///
void CommandLineInterface::handleArguments(int argc, char* argv[]) {
    int port = 33333; // Порт по умолчанию

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help") {
            printHelp();
            exit(0);
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "Версия программы: 1.0.0\n";
            exit(0);
        } else if (arg == "-p" && argc > 2) {
            port = std::stoi(argv[2]);
            std::cout << "Запуск сервера на порту: " << port << "\n";
            // Здесь можно добавить логику для запуска сервера
            exit(0);
        } else {
            std::cerr << "Неизвестная опция: " << arg << "\n";
            printHelp();
            exit(1);
        }
    }

    // Если нет аргументов, запускаем сервер с настройками по умолчанию
    std::cout << "Запуск сервера на порту: " << port << "\n";
    // Здесь можно добавить логику для запуска сервера
}
