#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
/**
* @brief Класс CommandLineInterface отвечает за интерфейс сервера
* @param printHelp
*/
class CommandLineInterface {
public:
    void printHelp() const;                     // Метод для вывода справки
    void handleArguments(int argc, char* argv[]); // Метод для обработки аргументов командной строки
};

#endif // INTERFACE_H
