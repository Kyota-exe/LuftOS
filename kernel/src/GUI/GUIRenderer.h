#ifndef LUFTOS_GUIRENDERER_H
#define LUFTOS_GUIRENDERER_H

#include <stdint.h>
#include "../Utilities/Math.h"
#include "../Framebuffer.h"
#include "../Rendering/GraphicsRenderer.h"
#include "GUIWindow.h"

class GUIRenderer
{
public:
    void NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    GUIRenderer(GraphicsRenderer* graphicsRenderer);

private:
    GraphicsRenderer* gRenderer;
};

#endif
