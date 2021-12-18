#ifndef LUFTOS_BMPIMAGEFILE_H
#define LUFTOS_BMPIMAGEFILE_H

#include <stdint.h>

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t bitmapSize;
    void* bitmapBuffer;
    uint32_t redBitMask;
    uint32_t greenBitMask;
    uint32_t blueBitMask;
    uint32_t alphaBitMask;
} BMPImage;

#endif
