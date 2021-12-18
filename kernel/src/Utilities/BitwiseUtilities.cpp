#include "BitwiseUtilities.h"

uint8_t TrailingZeroBitsRight(uint32_t bitPattern)
{
    uint8_t count = 32;
    if (bitPattern) count--;
    if (bitPattern & 0x0000ffff) count -= 16;
    if (bitPattern & 0x00ff00ff) count -= 8;
    if (bitPattern & 0x0f0f0f0f) count -= 4;
    if (bitPattern & 0x33333333) count -= 2;
    if (bitPattern & 0x55555555) count -= 1;
    return count;
}