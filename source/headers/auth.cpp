#include "auth.h"

// Определяем допустимые логин и пароль
const std::string Auth::validUsername = "user";
const std::string Auth::validPassword = "P@ssW0rd";

// Конструктор
Auth::Auth() {}

// Функция для авторизации
bool Auth::login(const std::string& username, const std::string& password) {
    return (username == validUsername && password == validPassword);
}
