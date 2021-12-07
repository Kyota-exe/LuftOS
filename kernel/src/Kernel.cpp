#include "BasicRenderer.h"
#include "StringConversion.h"

extern "C" void _start(Framebuffer* framebuffer, Psf1Font* psf1Font)
{
    BasicRenderer newRenderer = BasicRenderer(framebuffer, psf1Font);
    newRenderer.Print(ToString((uint64_t) 987654321));
    newRenderer.NewLine();
    newRenderer.Print(ToString((int64_t) -987654321));
    newRenderer.NewLine();
    newRenderer.Print(ToString((double)-3.14159));
    newRenderer.NewLine();
    newRenderer.Print(ToStringHex((uint64_t) 0x918236));
}