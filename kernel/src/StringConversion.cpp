#include "StringConversion.h"

// Buffer size is set to 32 because log10(2^64) = 19.27
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

char hexToStringOutput[32];
const char* ToStringHex(uint64_t value)
{
    if (value == 0) return "0x00";

    // Add "0x" to the beginning of the string output
    hexToStringOutput[0] = '0';
    hexToStringOutput[1] = 'x';

    uint8_t size = 0;
    uint64_t num = value;
    while (num)
    {
        size++;
        num /= 16;
    }

    for (uint8_t i = 2 + size; i > 2; --i)
    {
        uint8_t digit = value % 16;
        value /= 16;
        hexToStringOutput[i - 1] = "0123456789ABCDEF"[digit];
    }
    hexToStringOutput[2 + size] = 0;

    return hexToStringOutput;
}

// Buffer size is set to 32 because max decimal places is 20
char doubleToStringOutput[32];
const char* ToString(double value, uint8_t decimalPlaces)
{
    if (decimalPlaces > 20)
    {
        decimalPlaces = 20;
    }

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