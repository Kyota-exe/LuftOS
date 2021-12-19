#include "GUIRenderer.h"

const uint32_t GUIRenderer::WINDOW_HEADER_DEFAULT_FILL = 0xff1c1c1c;
const uint32_t GUIRenderer::WINDOW_DEFAULT_FILL = 0xf0383838;
const unsigned int GUIRenderer::WINDOW_HEADER_HEIGHT = 30;

// Window Close Icon (Temporary)
const uint32_t GUIRenderer::WINDOW_CLOSE_ICON_COLOUR = 0xff858585;
const int GUIRenderer::WINDOW_CLOSE_ICON_OFFSET_X_FROM_RIGHT = -20;
const int GUIRenderer::WINDOW_CLOSE_ICON_OFFSET_Y = 14;
const unsigned int GUIRenderer::WINDOW_CLOSE_ICON_HEIGHT = 10;
const unsigned int GUIRenderer::WINDOW_CLOSE_ICON_WIDTH = 10;
const int GUIRenderer::WINDOW_CLOSE_ICON_SEPARATOR_X_FROM_RIGHT = -40;
const uint32_t GUIRenderer::WINDOW_CLOSE_ICON_SEPARATOR_COLOUR = 0xff525252;

GUIRenderer::GUIRenderer(Framebuffer* _framebuffer): framebuffer(_framebuffer) { }

void GUIRenderer::PutPixel(unsigned int x, unsigned int y, uint32_t colour, bool alpha)
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

void GUIRenderer::FillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint32_t colour, bool alpha)
{
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

void GUIRenderer::DrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha)
{
    DDAAlgorithm(x0, y0, x1, y1, colour, alpha);
}

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    FillRect(x, y, width, WINDOW_HEADER_HEIGHT, WINDOW_HEADER_DEFAULT_FILL);
    FillRect(x, y + WINDOW_HEADER_HEIGHT, width, height - WINDOW_HEADER_HEIGHT, WINDOW_DEFAULT_FILL, true);

    // Draw "X" (window close icon) (Temporary)
    unsigned int closeIconPosX = x + width - 1 + WINDOW_CLOSE_ICON_OFFSET_X_FROM_RIGHT;
    unsigned int closeIconPosY = y + WINDOW_CLOSE_ICON_OFFSET_Y;
    DrawLine(closeIconPosX - WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY - WINDOW_CLOSE_ICON_HEIGHT / 2,
             closeIconPosX + WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY + WINDOW_CLOSE_ICON_HEIGHT / 2,
             WINDOW_CLOSE_ICON_COLOUR);
    DrawLine(closeIconPosX - WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY + WINDOW_CLOSE_ICON_HEIGHT / 2,
             closeIconPosX + WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY - WINDOW_CLOSE_ICON_HEIGHT / 2,
             WINDOW_CLOSE_ICON_COLOUR);
    // Close icon separator
    unsigned int closeIconSeparatorPos = x + width - 1 + WINDOW_CLOSE_ICON_SEPARATOR_X_FROM_RIGHT;
    DrawLine(closeIconSeparatorPos, y, closeIconSeparatorPos, y + WINDOW_HEADER_HEIGHT - 1, WINDOW_CLOSE_ICON_SEPARATOR_COLOUR);
}

void GUIRenderer::DDAAlgorithm(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint32_t colour, bool alpha)
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