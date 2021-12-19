#ifndef LUFTOS_GUIRENDERER_H
#define LUFTOS_GUIRENDERER_H

#include <stdint.h>

#include "../Utilities/Math.h"
#include "../Framebuffer.h"

class GUIRenderer
{
public:
    Framebuffer* framebuffer;

    void PutPixel(unsigned int x, unsigned int y, uint32_t colour, bool alpha = false);
    void FillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint32_t colour, bool alpha = false);
    void DrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha = false);
    void NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    GUIRenderer(Framebuffer* _framebuffer);

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

    void DDAAlgorithm(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha);
    //void BresenhamAlgorithm(UIntVector2 a, UIntVector2 b, uint32_t colour);
};

#endif
