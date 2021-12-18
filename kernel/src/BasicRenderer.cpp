#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Framebuffer* _targetFramebuffer, Psf1Font* _psf1Font,
                             const UIntVector2 _cursorPosition, const unsigned int _colour) :
                             targetFramebuffer(_targetFramebuffer), psf1Font(_psf1Font),
                             cursorPosition(_cursorPosition), colour(_colour) { }

void BasicRenderer::PutChar(char c, unsigned int xOffset, unsigned int yOffset)
{
    unsigned int* pixPtr = (unsigned int*)targetFramebuffer->baseAddress;
    char* glyphPtr = (char*)psf1Font->glyphBuffer + (c * psf1Font->psf1Header->charSize);
    // 16 and 8 (glyph pixel size) are hardcoded
    for (unsigned long y = yOffset; y < yOffset + CHAR_HEIGHT; y++)
    {
        for (unsigned long x = xOffset; x < xOffset + CHAR_WIDTH; x++)
        {
            if ((*glyphPtr & (0b10000000 >> (x - xOffset))) > 0)
            {
                *(unsigned int*)(pixPtr + x + (y * targetFramebuffer->pixelsPerScanLine)) = colour;
            }
        }
        glyphPtr++;
    }
}

void BasicRenderer::Print(const char* str)
{
    char* chr = (char*)str;
    while (*chr != 0)
    {
        // 16 and 8 (glyph pixel size) are hardcoded
        PutChar(*chr, cursorPosition.x, cursorPosition.y);
        cursorPosition.x += CHAR_WIDTH;
        // Prevent overflow
        if (cursorPosition.x + CHAR_WIDTH > targetFramebuffer->width)
        {
            NewLine();
        }
        chr++;
    }
}

void BasicRenderer::NewLine()
{
    cursorPosition.x = 0;
    cursorPosition.y += CHAR_HEIGHT;
}

void BasicRenderer::FullScreenRenderBMP(BMPImage* bmpImage)
{
    if (bmpImage->height != targetFramebuffer->height || bmpImage->width != targetFramebuffer->width)
    {
        uint32_t previousColour = colour;
        colour = 0xffff0000;
        Print("BMP image is not the same resolution as the screen resolution!");
        colour = previousColour;
    }

    for (unsigned long y = 0; y < bootInfo->bmpImage->height; ++y)
    {
        for (unsigned long x = 0; x < bootInfo->bmpImage->width; ++x)
        {
            unsigned int* framebufferPtr = (unsigned int*)bootInfo->framebuffer->baseAddress + bootInfo->bmpImage->width * y + x;
            unsigned int* pixPtr = (unsigned int*)bootInfo->bmpImage->bitmapBuffer + (bootInfo->bmpImage->height - 1 - y) * bootInfo->bmpImage->width + x;
            *ptr = *pixPtr;
        }
    }
}