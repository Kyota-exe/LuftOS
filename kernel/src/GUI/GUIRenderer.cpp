#include "GUIRenderer.h"

GUIRenderer::GUIRenderer(GraphicsRenderer* graphicsRenderer): gRenderer(graphicsRenderer) { }

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    GUIWindow window = GUIWindow(gRenderer, x, y, width, height);
    window.Draw();

    //window.basicRenderer->cursorPosition += UIntVector2(width / 2, height / 2);
    window.basicRenderer->cursorPosition = UIntVector2(x + width / 4 - 26, y + height / 4 - 6);
    window.basicRenderer->leftBound = x + width / 4 - 26;

    window.basicRenderer->fontSizeMultiplier = 2;
    const char* whale = R"""(       .
      ":"
    ___:____     |"\/"|
  ,'        `.    \  /
  |  O        \___/  |
~^~^~^~^~^~^~^~^~^~^~^~^~
)""";
    window.basicRenderer->Print(whale);

//    window.basicRenderer->fontSizeMultiplier = 1;
//    const char* title = R"""( _           __ _    ___  ____
//| |   _   _ / _| |_ / _ \/ ___|
//| |  | | | | |_| __| | | \___ \
//| |__| |_| |  _| |_| |_| |___) |
//|_____\__,_|_|  \__|\___/|____/
//)""";
//
//    window.basicRenderer->Print(title);
}