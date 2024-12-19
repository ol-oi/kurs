#include "calc.h"

///
/// @brief - Считает среднее арифметическое вектора
///
double Calculator::calculate_average(const std::vector<double>& vec) {
    if (vec.empty()) {
        return 0.0; // Возвращаем 0, если вектор пустой
    }

    double sum = 0.0;
    for (double value : vec) {
        sum += value;
    }
    return sum / vec.size();
}
