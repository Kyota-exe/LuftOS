#ifndef LUFTOS_BASICRENDERER_H
#define LUFTOS_BASICRENDERER_H

#include "Math.h"
#include "Framebuffer.h"
#include "SimpleFonts.h"

class BasicRenderer
{
public:
    Framebuffer* targetFramebuffer;
    Psf1Font* psf1Font;
    UIntVector2 cursorPosition;
    unsigned int colour;

    void Print(const char* str);
    void PutChar(char c, unsigned int xOffset, unsigned int yOffset);
    void NewLine();

    BasicRenderer(Framebuffer* _targetFramebuffer, Psf1Font* _psf1Font,
                  UIntVector2 _cursorPosition = {0, 0},
                  unsigned int _colour = 0xffffffff);

private:
    static const int CHAR_WIDTH = 8;
    static const int CHAR_HEIGHT = 16;
};

#endif
