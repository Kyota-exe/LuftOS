#include "GUIWindow.h"

const uint32_t GUIWindow::DEFAULT_FILL_COLOUR = 0xf0'38'38'38;

// Window Header
const uint32_t GUIWindow::HEADER_DEFAULT_FILL_COLOUR = 0xff'1c'1c'1c;
const unsigned int GUIWindow::HEADER_HEIGHT = 30;

// Window Close Icon (Temporary)
const uint32_t GUIWindow::CLOSE_ICON_COLOUR = 0xff'85'85'85;
const int GUIWindow::CLOSE_ICON_OFFSET_X_FROM_RIGHT = -20;
const int GUIWindow::CLOSE_ICON_OFFSET_Y = 14;
const unsigned int GUIWindow::CLOSE_ICON_HEIGHT = 10;
const unsigned int GUIWindow::CLOSE_ICON_WIDTH = 10;
const uint32_t GUIWindow::CLOSE_ICON_SEPARATOR_COLOUR = 0xff'52'52'52;
const int GUIWindow::CLOSE_ICON_SEPARATOR_X_FROM_RIGHT = -40;

// Window surrounding shadow
const int GUIWindow::SHADOW_LAYER_COUNT = 5;
const uint32_t GUIWindow::SHADOW_LAYER_COLOURS[SHADOW_LAYER_COUNT]
{
    0x64'00'00'00,
    0x50'00'00'00,
    0x3c'00'00'00,
    0x28'00'00'00,
    0x14'00'00'00
};

GUIWindow::GUIWindow(GraphicsRenderer* graphicsRenderer, unsigned int _x, unsigned int _y,
                     unsigned int _width, unsigned int _height): gRenderer(graphicsRenderer),
                     x(_x), y(_y), width(_width), height(_height) { }

void GUIWindow::Draw()
{
    unsigned int right = x + width - 1;
    unsigned int bottom = y + height - 1;

    gRenderer->FillRect(x, y, width, HEADER_HEIGHT, HEADER_DEFAULT_FILL_COLOUR);
    gRenderer->FillRect(x, y + HEADER_HEIGHT, width, height - HEADER_HEIGHT, DEFAULT_FILL_COLOUR, true);

    // Draw "X" (window close icon) (Temporary)
    unsigned int closeIconPosX = right + CLOSE_ICON_OFFSET_X_FROM_RIGHT;
    unsigned int closeIconPosY = y + CLOSE_ICON_OFFSET_Y;
    gRenderer->DrawLine(closeIconPosX - CLOSE_ICON_WIDTH / 2,
                        closeIconPosY - CLOSE_ICON_HEIGHT / 2,
                        closeIconPosX + CLOSE_ICON_WIDTH / 2,
                        closeIconPosY + CLOSE_ICON_HEIGHT / 2,
                        CLOSE_ICON_COLOUR);
    gRenderer->DrawLine(closeIconPosX - CLOSE_ICON_WIDTH / 2,
                        closeIconPosY + CLOSE_ICON_HEIGHT / 2,
                        closeIconPosX + CLOSE_ICON_WIDTH / 2,
                        closeIconPosY - CLOSE_ICON_HEIGHT / 2,
                        CLOSE_ICON_COLOUR);
    // Close icon separator
    unsigned int closeIconSeparatorPos = right + CLOSE_ICON_SEPARATOR_X_FROM_RIGHT;
    gRenderer->DrawLine(closeIconSeparatorPos, y, closeIconSeparatorPos, y + HEADER_HEIGHT - 1, CLOSE_ICON_SEPARATOR_COLOUR);

    // Shadow
    for (int i = 1; i <= SHADOW_LAYER_COUNT; ++i)
    {
        unsigned int shadowX = x - i;
        unsigned int shadowY = y - i;
        unsigned int shadowWidth = (right + i) - shadowX;
        unsigned int shadowHeight = (bottom + i) - shadowY;
        gRenderer->DrawRect(shadowX, shadowY, shadowWidth, shadowHeight, SHADOW_LAYER_COLOURS[i - 1], true);
    }
}