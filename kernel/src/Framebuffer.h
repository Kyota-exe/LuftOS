#ifndef LUFTOS_FRAMEBUFFER_H
#define LUFTOS_FRAMEBUFFER_H

#include <stddef.h>

struct Framebuffer
{
    unsigned int* baseAddress;
    size_t bufferSize;
    unsigned int width;
    unsigned int height;
    unsigned int pixelsPerScanLine;
};

#endif
