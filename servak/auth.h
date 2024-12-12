#ifndef AUTH_H
#define AUTH_H

#include <string>

class Auth {
public:
    // Конструктор
    Auth();

    // Функция для авторизации
    bool login(const std::string& username, const std::string& password);

private:
    // Статические данные для проверки логина и пароля
    static const std::string validUsername;
    static const std::string validPassword;
};

#endif // AUTH_H
