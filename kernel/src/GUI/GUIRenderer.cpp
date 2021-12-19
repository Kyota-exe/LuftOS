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

GUIRenderer::GUIRenderer(GraphicsRenderer* _graphicsRenderer): graphicsRenderer(_graphicsRenderer) { }

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    graphicsRenderer->FillRect(x, y, width, WINDOW_HEADER_HEIGHT, WINDOW_HEADER_DEFAULT_FILL);
    graphicsRenderer->FillRect(x, y + WINDOW_HEADER_HEIGHT, width, height - WINDOW_HEADER_HEIGHT, WINDOW_DEFAULT_FILL, true);

    // Draw "X" (window close icon) (Temporary)
    unsigned int closeIconPosX = x + width - 1 + WINDOW_CLOSE_ICON_OFFSET_X_FROM_RIGHT;
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
    unsigned int closeIconSeparatorPos = x + width - 1 + WINDOW_CLOSE_ICON_SEPARATOR_X_FROM_RIGHT;
    graphicsRenderer->DrawLine(closeIconSeparatorPos, y, closeIconSeparatorPos, y + WINDOW_HEADER_HEIGHT - 1, WINDOW_CLOSE_ICON_SEPARATOR_COLOUR);
}