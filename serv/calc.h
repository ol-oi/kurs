#ifndef CALC_H
#define CALC_H

#include <vector>
#include <cstdint>

class Calculator {
public:
    static int handleOverflow(int64_t sum, int numBits);
    static int calculateAverage(const std::vector<int32_t>& values);
};

#endif // CALC_H
