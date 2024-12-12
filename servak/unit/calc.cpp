#include <UnitTest++/UnitTest++.h>
#include "calc.h"

TEST(CalculateAverage_EmptyVector) {
    std::vector<double> vec;
    double result = Calculator::calculate_average(vec);
    CHECK_EQUAL(0.0, result);
}

TEST(CalculateAverage_SingleElement) {
    std::vector<double> vec = {5.0};
    double result = Calculator::calculate_average(vec);
    CHECK_EQUAL(5.0, result);
}

TEST(CalculateAverage_MultipleElements) {
    std::vector<double> vec = {1.0, 2.0, 3.0, 4.0, 5.0};
    double result = Calculator::calculate_average(vec);
    CHECK_EQUAL(3.0, result);
}

TEST(CalculateAverage_NegativeElements) {
    std::vector<double> vec = {-1.0, -2.0, -3.0};
    double result = Calculator::calculate_average(vec);
    CHECK_EQUAL(-2.0, result);
}

TEST(CalculateAverage_MixedElements) {
    std::vector<double> vec = {-1.0, 0.0, 1.0};
    double result = Calculator::calculate_average(vec);
    CHECK_EQUAL(0.0, result);
}

TEST(CalculateAverage_LargeNumbers) {
    std::vector<double> vec = {1e6, 2e6, 3e6};
    double result = Calculator::calculate_average(vec);
    CHECK_EQUAL(2e6, result);
}

TEST(CalculateAverage_Floats) {
    std::vector<double> vec = {1.5, 2.5, 3.5};
    double result = Calculator::calculate_average(vec);
    CHECK_CLOSE(2.5, result, 1e-9); // Используем CHECK_CLOSE для проверки с плавающей точкой
}

int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
