#include <commons.h>

double ModuloDouble(const double value, const double divisor)
{
    if (divisor == 0.0)
        return 0.0;

    const double quotient = (int)(value / divisor);
    return value - quotient * divisor;
}

float ModuloFloat(const float value, const float divisor)
{
    if (divisor == 0.0f)
        return 0.0f;

    const float quotient = (int)(value / divisor);
    return value - quotient * divisor;
}
