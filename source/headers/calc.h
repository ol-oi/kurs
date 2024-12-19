#ifndef CALC_H
#define CALC_H

#include <vector>
/**
* @brief Класс Calculator отвечает за вычисления сервера
*/
class Calculator {
public:
    static double calculate_average(const std::vector<double>& vec);
};

#endif // CALC_H
