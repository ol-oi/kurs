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

// Функция для запуска всех тестов
void run_tests() {
    UnitTest::RunAllTests();
}
