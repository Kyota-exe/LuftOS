#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Framebuffer* _targetFramebuffer, Psf1Font* _psf1Font,
                             const UIntVector2 _cursorPosition, const unsigned int _colour) :
                             targetFramebuffer(_targetFramebuffer), psf1Font(_psf1Font),
                             cursorPosition(_cursorPosition), colour(_colour) { }

void BasicRenderer::PutChar(char c, unsigned int xOffset, unsigned int yOffset)
{
    unsigned int* pixPtr = targetFramebuffer->baseAddress;
    char* glyphPtr = psf1Font->glyphBuffer + (c * psf1Font->psf1Header->charSize);
    // 16 and 8 (glyph pixel size) are hardcoded
    for (unsigned long y = yOffset; y < yOffset + CHAR_HEIGHT; y++)
    {
        for (unsigned long x = xOffset; x < xOffset + CHAR_WIDTH; x++)
        {
            if ((*glyphPtr & (0b10000000 >> (x - xOffset))) > 0)
            {
                *(pixPtr + x + (y * targetFramebuffer->pixelsPerScanLine)) = colour;
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