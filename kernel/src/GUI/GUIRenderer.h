#ifndef LUFTOS_GUIRENDERER_H
#define LUFTOS_GUIRENDERER_H

#include <stdint.h>

#include "../Utilities/Math.h"
#include "../Framebuffer.h"

class GUIRenderer
{
public:
    Framebuffer* framebuffer;

    void PutPixel(unsigned int x, unsigned int y, uint32_t colour);
    void FillRect(UIntVector2 topLeft, UIntVector2 bottomRight, uint32_t colour);
    void DrawLine(UIntVector2 a, UIntVector2 b, uint32_t colour);

    GUIRenderer(Framebuffer* _framebuffer);
};

#endif
