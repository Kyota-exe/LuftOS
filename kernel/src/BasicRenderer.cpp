#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Framebuffer* _targetFramebuffer, Psf1Font* _psf1Font, unsigned int _leftBound,
                             UIntVector2 _cursorPosition, uint32_t _colour, unsigned int _fontSizeMultiplier) :
                             targetFramebuffer(_targetFramebuffer), psf1Font(_psf1Font), leftBound(_leftBound),
                             cursorPosition(_cursorPosition), colour(_colour), fontSizeMultiplier(_fontSizeMultiplier) { colours0 = 0; colours1 = 0; }

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

void BasicRenderer::Print(const char* str, const char* end)
{
    char* chr = (char*)str;

    bool colorCodeNext = false;
    uint32_t previousColor = colour;

    while (*chr != 0)
    {
        if (colorCodeNext)
        {
            //colour = colours[(int)*chr];
            if (*chr - '0' == 0)
            {
                colour = colours0;
            }
            else if (*chr - '0' == 1)
            {
                colour = colours1;
            }
            colorCodeNext = false;
            chr++;
            continue;
        }
        else if (*chr == '\n')
        {
            NewLine();
            chr++;
            continue;
        }
        else if (*chr == '&')
        {
            colorCodeNext = true;
            chr++;
            continue;
        }
        PutChar(*chr, cursorPosition.x, cursorPosition.y);
        cursorPosition.x += CHAR_WIDTH * fontSizeMultiplier;
        // Prevent overflow
        if (cursorPosition.x + CHAR_WIDTH * fontSizeMultiplier > targetFramebuffer->width)
        {
            NewLine();
        }
        chr++;
    }

    colour = previousColor;

    if (end[0] != 0)
    {
        Print(end, "");
    }
}

void BasicRenderer::NewLine()
{
    cursorPosition.x = leftBound;
    cursorPosition.y += CHAR_HEIGHT * fontSizeMultiplier;
}