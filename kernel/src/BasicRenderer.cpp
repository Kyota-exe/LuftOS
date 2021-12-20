#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Framebuffer* _targetFramebuffer, Psf1Font* _psf1Font,
                             UIntVector2 _cursorPosition, uint32_t _colour, unsigned int _fontSizeMultiplier) :
                             targetFramebuffer(_targetFramebuffer), psf1Font(_psf1Font),
                             cursorPosition(_cursorPosition), colour(_colour), fontSizeMultiplier(_fontSizeMultiplier) { }

void BasicRenderer::PutChar(char c, unsigned int xOffset, unsigned int yOffset)
{
    unsigned int* pixPtr = targetFramebuffer->baseAddress;
    char* glyphPtr = psf1Font->glyphBuffer + (c * psf1Font->psf1Header->charSize);

    int glyphPtrIncrementStep = 0;
    for (unsigned int y = yOffset; y < yOffset + CHAR_HEIGHT * fontSizeMultiplier; y++)
    {
        for (unsigned int x = xOffset; x < xOffset + CHAR_WIDTH * fontSizeMultiplier; x++)
        {
            if ((*glyphPtr & (0b10000000 >> ((x - xOffset) / fontSizeMultiplier))))
            {
                *(pixPtr + x + (y * targetFramebuffer->pixelsPerScanLine)) = colour;
            }
        }
        glyphPtrIncrementStep++;
        if (glyphPtrIncrementStep == fontSizeMultiplier)
        {
            glyphPtr++;
            glyphPtrIncrementStep = 0;
        }
    }
}

void BasicRenderer::Print(const char* str)
{
    char* chr = (char*)str;
    while (*chr != 0)
    {
        // 16 and 8 (glyph pixel size) are hardcoded
        PutChar(*chr, cursorPosition.x, cursorPosition.y);
        cursorPosition.x += CHAR_WIDTH * fontSizeMultiplier;
        // Prevent overflow
        if (cursorPosition.x + CHAR_WIDTH * fontSizeMultiplier > targetFramebuffer->width)
        {
            NewLine();
        }
        chr++;
    }
}

void BasicRenderer::NewLine()
{
    cursorPosition.x = 0;
    cursorPosition.y += CHAR_HEIGHT * fontSizeMultiplier;
}