#include "GUIRenderer.h"

GUIRenderer::GUIRenderer(GraphicsRenderer* graphicsRenderer): gRenderer(graphicsRenderer) { }

void GUIRenderer::NewWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    GUIWindow window = GUIWindow(gRenderer, x, y, width, height);
    window.Draw();
    window.basicRenderer->Print("Hello world");
}