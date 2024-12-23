#include <UnitTest++/UnitTest++.h>
#include "../../servak/source/headers/auth.h"
#include "../../servak/source/headers/calc.h"
#include "../../servak/source/headers/logger.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <filesystem>


TEST(CalculateAverage_EmptyVector) {
    Calculator calc;
    std::vector<double> vec;
    CHECK_EQUAL(0.0, calc.calculate_average(vec));
}

TEST(CalculateAverage_SingleElement) {
    Calculator calc;
    std::vector<double> vec = {42.0};
    CHECK_EQUAL(42.0, calc.calculate_average(vec));
}

TEST(CalculateAverage_MultipleElements) {
    Calculator calc;
    std::vector<double> vec = {10.0, 20.0, 30.0};
    CHECK_EQUAL(20.0, calc.calculate_average(vec));
}

TEST(CalculateAverage_NegativeAndPositive) {
    Calculator calc;
    std::vector<double> vec = {-10.0, 0.0, 10.0};
    CHECK_EQUAL(0.0, calc.calculate_average(vec));
}

TEST(CalculateAverage_Floats) {
    Calculator calc;
    std::vector<double> vec = {1.5, 2.5, 3.5};
    CHECK_CLOSE(2.5, calc.calculate_average(vec), 1e-5); // Используем CHECK_CLOSE для проверки с плавающей точкой
}


TEST(Auth_ValidLogin) {
    Auth auth;
    CHECK(auth.login("user", "P@ssW0rd"));  // Проверяем правильные логин и пароль
}

TEST(Auth_InvalidUsername) {
    Auth auth;
    CHECK(!auth.login("wrongUser", "P@ssW0rd"));  // Проверяем неправильный логин
}

TEST(Auth_InvalidPassword) {
    Auth auth;
    CHECK(!auth.login("user", "wrongPassword"));  // Проверяем неправильный пароль
}

TEST(Auth_InvalidCredentials) {
    Auth auth;
    CHECK(!auth.login("wrongUser", "wrongPassword"));  // Проверяем неправильные логин и пароль
}

TEST(Auth_EmptyCredentials) {
    Auth auth;
    CHECK(!auth.login("", ""));  // Проверяем пустые логин и пароль
}

int main(int argc, char** argv) {
    return UnitTest::RunAllTests();  // Запуск всех тестов
}
