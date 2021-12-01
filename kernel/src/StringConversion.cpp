#include "StringConversion.h"

char uintToStringOutput[32];
const char* ToString(uint64_t value)
{
    if (value == 0) return "0";

    uint8_t size = 0;
    uint64_t num = value;
    while (num)
    {
        size++;
        num /= 10;
    }

    for (uint8_t i = size; i > 0; --i)
    {
        uint8_t digit = value % 10;
        value /= 10;
        uintToStringOutput[i - 1] = digit + '0';
    }
    uintToStringOutput[size] = 0;

    return uintToStringOutput;
}

char intToStringOutput[32];
const char* ToString(int64_t value)
{
    if (value == 0) return "0";

    uint8_t isNegative = 0;
    if (value < 0)
    {
        isNegative = 1;
        value *= -1;
        intToStringOutput[0] = '-';
    }

    uint8_t size = 0;
    uint64_t num = value;
    while (num)
    {
        size++;
        num /= 10;
    }

    for (uint8_t i = size; i > 0; --i)
    {
        uint8_t digit = value % 10;
        value /= 10;
        intToStringOutput[isNegative + i - 1] = digit + '0';
    }
    intToStringOutput[isNegative + size] = 0;

    return intToStringOutput;
}

char doubleToStringOutput[32];
const char* ToString(double value, uint8_t decimalPlaces)
{
    char* intPtr = (char*)ToString((int64_t)value);
    char* doublePtr = doubleToStringOutput;

    if (value < 0)
    {
        value *= -1;
    }

    while (*intPtr != 0)
    {
        *doublePtr = *intPtr;
        doublePtr++, intPtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double decimals = value - (int)value;

    for (uint8_t i = 0; i < decimalPlaces; ++i)
    {
        decimals *= 10;
        *doublePtr = (int)decimals + '0';
        decimals -= (int)decimals;
        doublePtr++;
    }

    *doublePtr = 0;

    return doubleToStringOutput;
}