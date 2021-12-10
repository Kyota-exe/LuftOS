#include "Bitmap.h"

bool Bitmap::operator[](uint64_t index)
{
    // Each element of the bitmap is 1 byte
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    return (buffer[byteIndex] & bitIndexer) > 0;
}

void Bitmap::Set(uint64_t index, bool value)
{
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
}
