#ifndef LUFTOS_GUIRENDERER_H
#define LUFTOS_GUIRENDERER_H

#include <stdint.h>
#include "../Utilities/Math.h"
#include "../Framebuffer.h"
#include "../Rendering/GraphicsRenderer.h"

class GUIRenderer
{
public:
    GraphicsRenderer* graphicsRenderer;
    void NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    GUIRenderer(GraphicsRenderer* _graphicsRenderer);

private:
    static const uint32_t WINDOW_HEADER_DEFAULT_FILL;
    static const uint32_t WINDOW_DEFAULT_FILL;
    static const unsigned int WINDOW_HEADER_HEIGHT;

    // Window Close Icon (Temporary)
    static const uint32_t WINDOW_CLOSE_ICON_COLOUR;
    static const int WINDOW_CLOSE_ICON_OFFSET_X_FROM_RIGHT;
    static const int WINDOW_CLOSE_ICON_OFFSET_Y;
    static const unsigned int WINDOW_CLOSE_ICON_HEIGHT;
    static const unsigned int WINDOW_CLOSE_ICON_WIDTH;
    static const int WINDOW_CLOSE_ICON_SEPARATOR_X_FROM_RIGHT;
    static const uint32_t WINDOW_CLOSE_ICON_SEPARATOR_COLOUR;

    // Window surrounding shadow
    static const int WINDOW_SHADOW_LAYER_COUNT;
    static const uint32_t WINDOW_SHADOW_LAYER_COLOURS[];
};

#endif
