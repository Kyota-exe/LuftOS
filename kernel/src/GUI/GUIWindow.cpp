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

// Window Maximize Icon (Temporary)
const uint32_t GUIWindow::MAXIMIZE_ICON_COLOUR = 0xff'85'85'85;
const int GUIWindow::MAXIMIZE_ICON_OFFSET_X_FROM_RIGHT = -60;
const int GUIWindow::MAXIMIZE_ICON_OFFSET_Y = 14;
const unsigned int GUIWindow::MAXIMIZE_ICON_HEIGHT = 10;
const unsigned int GUIWindow::MAXIMIZE_ICON_WIDTH = 10;

// Window Minimize Icon (Temporary)
const uint32_t GUIWindow::MINIMIZE_ICON_COLOUR = 0xff'85'85'85;
const int GUIWindow::MINIMIZE_ICON_OFFSET_X_FROM_RIGHT = -100;
const int GUIWindow::MINIMIZE_ICON_OFFSET_Y = 14;
const unsigned int GUIWindow::MINIMIZE_ICON_WIDTH = 10;

// Window surrounding shadow
const uint32_t GUIWindow::SHADOW_COLOR = 0xff'00'00'00;
const int GUIWindow::SHADOW_LAYER_COUNT = 10;
const int GUIWindow::MAX_SHADOW_OPACITY = 180;

GUIWindow::GUIWindow(GraphicsRenderer* graphicsRenderer, unsigned int _x, unsigned int _y, unsigned int _width,
                     unsigned int _height): gRenderer(graphicsRenderer), x(_x), y(_y), width(_width), height(_height)
{
    *basicRenderer = BasicRenderer(framebuffer, psf1Font, x , {x, y + HEADER_HEIGHT});
}

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

    // Maximize Icon
    gRenderer->rectMode = Center;
    gRenderer->DrawRect(right + MAXIMIZE_ICON_OFFSET_X_FROM_RIGHT, y + MAXIMIZE_ICON_OFFSET_Y,
                        MAXIMIZE_ICON_WIDTH, MAXIMIZE_ICON_HEIGHT, MAXIMIZE_ICON_COLOUR);

    // Minimize Icon
    gRenderer->DrawLine(right + MINIMIZE_ICON_OFFSET_X_FROM_RIGHT - MINIMIZE_ICON_WIDTH / 2, y + CLOSE_ICON_OFFSET_Y,
                        right + MINIMIZE_ICON_OFFSET_X_FROM_RIGHT + MINIMIZE_ICON_WIDTH / 2, y + CLOSE_ICON_OFFSET_Y,
                        MINIMIZE_ICON_COLOUR);

    // Shadow
    gRenderer->rectMode = Corner;
    for (int i = 1; i <= SHADOW_LAYER_COUNT; ++i)
    {
        unsigned int shadowX = x - i;
        unsigned int shadowY = y - i;
        unsigned int shadowWidth = (right + i) - shadowX;
        unsigned int shadowHeight = (bottom + i) - shadowY;

        char alpha = MAX_SHADOW_OPACITY - i * MAX_SHADOW_OPACITY / SHADOW_LAYER_COUNT;
        gRenderer->DrawRect(shadowX, shadowY, shadowWidth, shadowHeight, alpha << 24 | 0x00'00'00'00, true);
    }
}