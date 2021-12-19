#ifndef LUFTOS_GRAPHICSRENDERER_H
#define LUFTOS_GRAPHICSRENDERER_H

#include <stdint.h>
#include "../Framebuffer.h"
#include "../Utilities/Math.h"

class GraphicsRenderer
{
public:
    Framebuffer* framebuffer;
    void PutPixel(unsigned int x, unsigned int y, uint32_t colour, bool alpha = false);
    void FillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint32_t colour, bool alpha = false);
    void DrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha = false);
    GraphicsRenderer(Framebuffer* _framebuffer);

private:
    void DDAAlgorithm(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha);
    //void BresenhamAlgorithm(UIntVector2 a, UIntVector2 b, uint32_t colour);
};

#endif
