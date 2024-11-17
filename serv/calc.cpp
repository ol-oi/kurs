#include "calc.h"

int handleOverflow(int64_t sum, int numBits) {
    if (sum > INT32_MAX) {
        return INT32_MAX; // Возвращаем максимальное значение при переполнении вверх
    } else if (sum < INT32_MIN) {
        return -2 * (1 << (numBits - 1)); // Возвращаем -2n при переполнении вниз
    }
    return static_cast<int>(sum);
}

int calculateAverage(const std::vector<int32_t>& values) {
    int64_t sum = 0;
    for (int32_t value : values) {
        sum += value;
    }
    return handleOverflow(sum / values.size(), sizeof(int32_t) * 8);
}
