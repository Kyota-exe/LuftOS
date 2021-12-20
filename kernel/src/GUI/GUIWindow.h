#ifndef LUFTOS_GUIWINDOW_H
#define LUFTOS_GUIWINDOW_H

#include "GUIRenderer.h"
#include "../BasicRenderer.h"

// Temporarily for framebuffer
#include "../Kernel.h"

class GUIWindow
{
public:
    BasicRenderer* basicRenderer;

    void Draw();
    GUIWindow(GraphicsRenderer* graphicsRenderer,
              unsigned int _x,
              unsigned int _y,
              unsigned int _width,
              unsigned int _height);

private:
    GraphicsRenderer* gRenderer;

    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;

    static const uint32_t DEFAULT_FILL_COLOUR;

    // Window Header
    static const uint32_t HEADER_DEFAULT_FILL_COLOUR;
    static const unsigned int HEADER_HEIGHT;

    // Window Close Icon (Temporary)
    static const uint32_t CLOSE_ICON_COLOUR;
    static const int CLOSE_ICON_OFFSET_X_FROM_RIGHT;
    static const int CLOSE_ICON_OFFSET_Y;
    static const unsigned int CLOSE_ICON_HEIGHT;
    static const unsigned int CLOSE_ICON_WIDTH;
    static const uint32_t CLOSE_ICON_SEPARATOR_COLOUR;
    static const int CLOSE_ICON_SEPARATOR_X_FROM_RIGHT;

    // Window surrounding shadow
    static const int SHADOW_LAYER_COUNT;
    static const uint32_t SHADOW_LAYER_COLOURS[];
};

#endif
