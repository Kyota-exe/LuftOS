#ifndef LUFTOS_MATH_H
#define LUFTOS_MATH_H

struct UIntVector2
{
    unsigned int x;
    unsigned int y;
    UIntVector2(unsigned int _x, unsigned int _y);
    UIntVector2 operator+(UIntVector2 operand);
    UIntVector2 operator-(UIntVector2 operand);
};

int abs(int n);
int round(float n);

#endif
