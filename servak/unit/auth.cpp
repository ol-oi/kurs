#include <UnitTest++.h>
#include "auth.h"

// Тест для успешного входа
TEST(AuthSuccessfulLoginTest) {
    Auth auth;
    CHECK(auth.login("user", "P@ssW0rd"));
}

// Тест для неудачного входа с неправильным логином
TEST(AuthFailedLoginWithWrongUsernameTest) {
    Auth auth;
    CHECK(!auth.login("wrongUser", "P@ssW0rd"));
}

// Тест для неудачного входа с неправильным паролем
TEST(AuthFailedLoginWithWrongPasswordTest) {
    Auth auth;
    CHECK(!auth.login("user", "wrongPassword"));
}

// Тест для неудачного входа с неправильным логином и паролем
TEST(AuthFailedLoginWithBothWrongTest) {
    Auth auth;
    CHECK(!auth.login("wrongUser", "wrongPassword"));
}

// Основная функция для запуска тестов
int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
