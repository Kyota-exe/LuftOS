#include "GraphicsRenderer.h"

GraphicsRenderer::GraphicsRenderer(Framebuffer* _framebuffer): framebuffer(_framebuffer) { }

void GraphicsRenderer::PutPixel(unsigned int x, unsigned int y, uint32_t colour, bool alpha)
{
    unsigned int* ptr = framebuffer->baseAddress + framebuffer->width * y + x;
    if (!alpha)
    {
        *ptr = colour;
        return;
    }

    uint32_t destPixel = *ptr;
    uint8_t alphaValue = (colour >> 24) & 0xff;

    uint8_t destRed = (destPixel >> 16) & 0xff;
    uint8_t resultRed = alphaValue * (((colour >> 16) & 0xff) - destRed) / 256 + destRed;

    uint8_t destGreen = (destPixel >> 8) & 0xff;
    uint8_t resultGreen = alphaValue * (((colour >> 8) & 0xff) - destGreen) / 256 + destGreen;

    uint8_t destBlue = destPixel & 0xff;
    uint8_t resultBlue = alphaValue * ((colour & 0xff) - destBlue) / 256 + destBlue;

    *ptr = resultBlue | (resultGreen << 8) | (resultRed << 16);
}

void GraphicsRenderer::FillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint32_t colour, bool alpha)
{
    // Check if rectangle is out of screen bounds
    if (x >= framebuffer->width || y >= framebuffer->height || x + width >= framebuffer->width || y + height >= framebuffer->height)
    {
        // Error
        return;
    }

    for (unsigned int currentY = y; currentY < y + height; ++currentY)
    {
        for (unsigned int currentX = x; currentX < x + width; ++currentX)
        {
            PutPixel(currentX, currentY, colour, alpha);
        }
    }
}

void GraphicsRenderer::DrawRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint32_t colour, bool alpha)
{
    unsigned int right = x + width - 1;
    unsigned int bottom = y + height - 1;

    DrawLine(x, y, right, y, colour, alpha); // Top
    DrawLine(x, bottom, right, bottom, colour, alpha); // Bottom
    DrawLine(x, y + 1, x, bottom - 1, colour, alpha); // Left
    DrawLine(right, y + 1, right, bottom - 1, colour, alpha); // Right
}

void GraphicsRenderer::DrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha)
{
    DDAAlgorithm(x0, y0, x1, y1, colour, alpha);
}

void GraphicsRenderer::DDAAlgorithm(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha)
{
    // Digital Differential Analyzer (DDA) Line Drawing Algorithm

    int dx = x1 - x0;
    int dy = y1 - y0;

    int step;
    if (abs(dx) > abs(dy))
    {
        step = abs(dx);
    }
    else
    {
        step = abs(dy);
    }

    float xIncrement = (float)dx / step;
    float yIncrement = (float)dy / step;

    float x = x0;
    float y = y0;
    for (int i = 0; i <= step; ++i)
    {
        PutPixel(round(x), round(y), colour, alpha);
        x += xIncrement;
        y += yIncrement;
    }
}