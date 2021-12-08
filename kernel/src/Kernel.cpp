#include "BasicRenderer.h"
#include "StringConversion.h"
#include "EfiMemory.h"

struct BootInfo
{
    Framebuffer* framebuffer;
    Psf1Font* psf1Font;
    void* memMap;
    uint64_t memMapSize;
    uint64_t memDescSize;
};

extern "C" void _start(BootInfo* bootInfo)
{
    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1Font);

    uint64_t memMapEntryCount = bootInfo->memMapSize / bootInfo->memDescSize;
    for (int i = 0; i < memMapEntryCount; i++)
    {
        EfiMemoryDescriptor* desc = (EfiMemoryDescriptor*)((uint64_t)bootInfo->memMap + (i * bootInfo->memDescSize));

        newRenderer.Print(EFI_MEMORY_TYPES[desc->type]);
        newRenderer.colour = 0xa9a9a9;
        newRenderer.Print(" ");
        newRenderer.Print(ToString(desc->pagesCount * 4096 / 1024));
        newRenderer.Print(" KB");
        newRenderer.colour = 0xffffffff;

        newRenderer.NewLine();
    }
}