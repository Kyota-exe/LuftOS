#ifndef LUFTOS_BASICRENDERER_H
#define LUFTOS_BASICRENDERER_H

#include "Utilities/Math.h"
#include "Framebuffer.h"
#include "SimpleFonts.h"
#include "BMPImageFile.h"
#include <stdint.h>

class BasicRenderer
{
public:
    Framebuffer* targetFramebuffer;
    Psf1Font* psf1Font;
    UIntVector2 cursorPosition;
    uint32_t colour;
    int fontSizeMultiplier;
    int leftBound;

    // Temporarily, until a heap is implemented so that this can be an array
    uint32_t colours0;
    uint32_t colours1;

    static const int CHAR_WIDTH = 8;
    static const int CHAR_HEIGHT = 16;

    void Print(const char* str, const char* end = "\n");
    void PutChar(char c, unsigned int xOffset, unsigned int yOffset);
    void NewLine();

    BasicRenderer(Framebuffer* _targetFramebuffer, Psf1Font* _psf1Font,
                  unsigned int _leftBound, UIntVector2 _cursorPosition = {0, 0},
                  uint32_t _colour = 0xffffffff, unsigned int _fontSizeMultiplier = 1);
};

#endif
