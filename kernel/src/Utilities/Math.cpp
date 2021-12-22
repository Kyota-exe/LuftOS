#include "Math.h"

UIntVector2::UIntVector2(unsigned int _x, unsigned int _y): x(_x), y(_y) { }

UIntVector2 UIntVector2::operator+(UIntVector2 operand)
{
    return UIntVector2(x + operand.x, y + operand.y);
}

UIntVector2& UIntVector2::operator+=(const UIntVector2& right)
{
    this->x += right.x;
    this->y += right.y;
    return *this;
}

UIntVector2 UIntVector2::operator-(UIntVector2 operand)
{
    return UIntVector2(x - operand.x, y - operand.y);
}

UIntVector2& UIntVector2::operator-=(const UIntVector2& right)
{
    this->x -= right.x;
    this->y -= right.y;
    return *this;
}

int abs(int n)
{
    return n < 0 ? -n : n;
}

int round(float n)
{
    if (n >= 0.0f)
    {
        // Casting to int truncates the decimal places
        return (int)(n + 0.5);
    }
    else
    {
        return (int)(n - 0.5);
    }
}

unsigned short lfsr = 0x38af;
unsigned int bit;
unsigned int rand()
{
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
    return lfsr = (lfsr >> 1) | (bit << 15);
}