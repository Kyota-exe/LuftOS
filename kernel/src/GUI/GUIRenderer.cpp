#include "GUIRenderer.h"

const uint32_t GUIRenderer::WINDOW_HEADER_DEFAULT_FILL = 0xff'1c'1c'1c;
const uint32_t GUIRenderer::WINDOW_DEFAULT_FILL = 0xf0'38'38'38;
const unsigned int GUIRenderer::WINDOW_HEADER_HEIGHT = 30;

// Window Close Icon (Temporary)
const uint32_t GUIRenderer::WINDOW_CLOSE_ICON_COLOUR = 0xff'85'85'85;
const int GUIRenderer::WINDOW_CLOSE_ICON_OFFSET_X_FROM_RIGHT = -20;
const int GUIRenderer::WINDOW_CLOSE_ICON_OFFSET_Y = 14;
const unsigned int GUIRenderer::WINDOW_CLOSE_ICON_HEIGHT = 10;
const unsigned int GUIRenderer::WINDOW_CLOSE_ICON_WIDTH = 10;
const int GUIRenderer::WINDOW_CLOSE_ICON_SEPARATOR_X_FROM_RIGHT = -40;
const uint32_t GUIRenderer::WINDOW_CLOSE_ICON_SEPARATOR_COLOUR = 0xff'52'52'52;

// Window surrounding shadow
const int GUIRenderer::WINDOW_SHADOW_LAYER_COUNT = 5;
const uint32_t GUIRenderer::WINDOW_SHADOW_LAYER_COLOURS[WINDOW_SHADOW_LAYER_COUNT]
{
    0x64'00'00'00,
    0x50'00'00'00,
    0x3c'00'00'00,
    0x28'00'00'00,
    0x14'00'00'00
};

GUIRenderer::GUIRenderer(GraphicsRenderer* _graphicsRenderer): graphicsRenderer(_graphicsRenderer) { }

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    unsigned int right = x + width - 1;
    unsigned int bottom = y + height - 1;

    graphicsRenderer->FillRect(x, y, width, WINDOW_HEADER_HEIGHT, WINDOW_HEADER_DEFAULT_FILL);
    graphicsRenderer->FillRect(x, y + WINDOW_HEADER_HEIGHT, width, height - WINDOW_HEADER_HEIGHT, WINDOW_DEFAULT_FILL, true);

    // Draw "X" (window close icon) (Temporary)
    unsigned int closeIconPosX = right + WINDOW_CLOSE_ICON_OFFSET_X_FROM_RIGHT;
    unsigned int closeIconPosY = y + WINDOW_CLOSE_ICON_OFFSET_Y;
    graphicsRenderer->DrawLine(closeIconPosX - WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY - WINDOW_CLOSE_ICON_HEIGHT / 2,
             closeIconPosX + WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY + WINDOW_CLOSE_ICON_HEIGHT / 2,
             WINDOW_CLOSE_ICON_COLOUR);
    graphicsRenderer->DrawLine(closeIconPosX - WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY + WINDOW_CLOSE_ICON_HEIGHT / 2,
             closeIconPosX + WINDOW_CLOSE_ICON_WIDTH / 2,
             closeIconPosY - WINDOW_CLOSE_ICON_HEIGHT / 2,
             WINDOW_CLOSE_ICON_COLOUR);
    // Close icon separator
    unsigned int closeIconSeparatorPos = right + WINDOW_CLOSE_ICON_SEPARATOR_X_FROM_RIGHT;
    graphicsRenderer->DrawLine(closeIconSeparatorPos, y, closeIconSeparatorPos, y + WINDOW_HEADER_HEIGHT - 1, WINDOW_CLOSE_ICON_SEPARATOR_COLOUR);

    // Shadow
    for (int i = 1; i <= WINDOW_SHADOW_LAYER_COUNT; ++i)
    {
        unsigned int shadowX = x - i;
        unsigned int shadowY = y - i;
        unsigned int shadowWidth = (right + i) - shadowX;
        unsigned int shadowHeight = (bottom + i) - shadowY;
        graphicsRenderer->DrawRect(shadowX, shadowY, shadowWidth, shadowHeight, WINDOW_SHADOW_LAYER_COLOURS[i - 1], true);
    }
}