#include <iostream>
#include <string>
#include <stdexcept>
#include <openssl/rand.h>
#include <openssl/md5.h>
#include "UnitTest++.h"

// Объявляем функции, которые будем тестировать
std::string generate_salt();
std::string hash_password(const std::string& password, const std::string& salt);

// Тесты для функции generate_salt
TEST(GenerateSaltTest) {
    // Проверяем, что соль имеет правильную длину
    std::string salt = generate_salt();
    CHECK_EQUAL(salt.length(), 16); // 8 байт * 2 (шестнадцатеричный формат)

    // Проверяем, что соль не пустая
    CHECK(!salt.empty());
}

// Тесты для функции hash_password
TEST(HashPasswordTest) {
    std::string password = "my_secure_password";
    std::string salt = "12345678"; // Пример соли

    std::string hashed_password = hash_password(password, salt);

    // Ожидаемый хеш для "my_secure_password12345678"
    unsigned char expected_digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>( (password + salt).c_str()), 
        (password + salt).size(), expected_digest);

    std::ostringstream expected_hash_stream;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        expected_hash_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(expected_digest[i]);
    }

    CHECK_EQUAL(hashed_password, expected_hash_stream.str());
}

// Тест на обработку ошибки в функции generate_salt
TEST(GenerateSaltErrorTest) {
    // Мы не можем напрямую вызвать ошибку в OpenSSL, но можем проверить,
    // что функция не выбрасывает исключение при нормальных условиях.
    CHECK_NO_THROW(generate_salt());
}

// Основная функция для запуска тестов
int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
