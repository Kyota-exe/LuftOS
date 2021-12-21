#include "GUIRenderer.h"

GUIRenderer::GUIRenderer(GraphicsRenderer* graphicsRenderer): gRenderer(graphicsRenderer) { }

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    GUIWindow window = GUIWindow(gRenderer, x, y, width, height);
    window.Draw();

    window.basicRenderer->cursorPosition = UIntVector2(x + width / 4 - 26, y + height / 4 - 6);
    window.basicRenderer->leftBound = x + width / 4 - 26;

    window.basicRenderer->fontSizeMultiplier = 2;
    const char* whale = R"""(       .
      ":"
    ___:____     |"\/"|
  ,'        `.    \  /
  |  O        \___/  |
~^~^~^~^~^~^~^~^~^~^~^~^~)""";
    window.basicRenderer->Print(whale);
    window.basicRenderer->fontSizeMultiplier = 4;

    UIntVector2 offset = UIntVector2(100, 0);
    window.basicRenderer->cursorPosition += offset;

    window.basicRenderer->Print("Luf", "");
    offset.x = 7;
    window.basicRenderer->cursorPosition -= offset;
    window.basicRenderer->Print("tOS");
}