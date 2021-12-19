#include "GUIRenderer.h"

GUIRenderer::GUIRenderer(Framebuffer* _framebuffer): framebuffer(_framebuffer) { }

void GUIRenderer::PutPixel(unsigned int x, unsigned int y, uint32_t colour)
{
    *(framebuffer->baseAddress + framebuffer->width * y + x) = colour;
}

void GUIRenderer::FillRect(UIntVector2 topLeft, UIntVector2 bottomRight, uint32_t colour)
{
    for (unsigned int y = topLeft.y; y <= bottomRight.y; ++y)
    {
        for (unsigned int x = topLeft.x; x <= bottomRight.x; ++x)
        {
            PutPixel(x, y, colour);
        }
    }
}

void GUIRenderer::DrawLine(UIntVector2 a, UIntVector2 b, uint32_t colour)
{
    // This is a temporary DrawLine method that does not support diagonal lines
    if (a.x == b.x) // Vertical line
    {
        for (unsigned int y = (a.y < b.y) ? a.y : b.y; y <= (a.y > b.y) ? a.y : b.y; ++y)
        {
            PutPixel(a.x, y, colour);
        }
    }
    else // Horizontal line
    {
        for (unsigned int x = (a.x < b.x) ? a.x : b.x; x <= (a.x > b.x) ? a.x : b.x; ++x)
        {
            PutPixel(x, a.y, colour);
        }
    }
}