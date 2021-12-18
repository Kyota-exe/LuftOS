#ifndef LUFTOS_GUIRENDERER_H
#define LUFTOS_GUIRENDERER_H

#include "../Utilities/Math.h"
#include "../Framebuffer.h"

class GUIRenderer
{
public:
    Framebuffer* framebuffer;
    void DrawBox(UIntVector2 topLeft, UIntVector2 bottomRight);
    GUIRenderer(Framebuffer* _framebuffer);
};

#endif
