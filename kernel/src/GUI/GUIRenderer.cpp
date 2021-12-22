#include "GUIRenderer.h"

GUIRenderer::GUIRenderer(GraphicsRenderer* graphicsRenderer): gRenderer(graphicsRenderer) { }

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    GUIWindow window = GUIWindow(gRenderer, x, y, width, height);
    window.Draw();

    window.basicRenderer->cursorPosition = UIntVector2(x + width / 2 - 26 * window.basicRenderer->CHAR_WIDTH * 2 / 2,
                                                       y + height / 2 - (6 * window.basicRenderer->CHAR_HEIGHT * 2 + 1 * window.basicRenderer->CHAR_HEIGHT * 4) / 2);
    window.basicRenderer->leftBound = window.basicRenderer->cursorPosition.x;

    window.basicRenderer->fontSizeMultiplier = 2;
    window.basicRenderer->colours0 = 0xff'00c0ff;
    window.basicRenderer->colours1 = 0xff'00ecff;
    const char* whale = R"""(&0       .
      ":"
    &1___&0:&1____     |"\/"|
  ,'        `.    \  /
  |  O        \___/  |
&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~&1^&0~)""";
    window.basicRenderer->Print(whale);

    window.basicRenderer->fontSizeMultiplier = 4;
    window.basicRenderer->cursorPosition.x += 100;
    window.basicRenderer->colours0 = 0xff'8FECFF;
    window.basicRenderer->colours1 = 0xff'73E1FF;
    window.basicRenderer->Print("&0Luf", "");
    window.basicRenderer->cursorPosition.x -= 7;
    window.basicRenderer->Print("&0t&1OS");
}