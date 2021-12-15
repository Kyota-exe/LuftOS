#include "Bitmap.h"

bool Bitmap::operator[](uint64_t index)
{
    if (index > size * 8) return false;

    // Each element of the bitmap is 1 byte
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    return (buffer[byteIndex] & bitIndexer) > 0;
}

bool Bitmap::Set(uint64_t index, bool value)
{
    if (index > size * 8) return false;

    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    if (value)
    {
        buffer[byteIndex] |= bitIndexer;
    }
    else
    {
        buffer[byteIndex] &= ~bitIndexer;
    }

    return true;
}
