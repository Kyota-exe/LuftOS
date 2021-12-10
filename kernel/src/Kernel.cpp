#include "BasicRenderer.h"
#include "StringConversion.h"
#include "Memory/EfiMemory.h"
#include "Memory/Memory.h"
#include "Bitmap.h"
#include "Memory/PageFrameAllocator.h"

struct BootInfo
{
    Framebuffer* framebuffer;
    Psf1Font* psf1Font;
    EfiMemoryDescriptor* memMap;
    uint64_t memMapSize;
    uint64_t memDescSize;
};

extern uint64_t kernelStart;
extern uint64_t kernelEnd;

extern "C" void _start(BootInfo* bootInfo)
{
    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1Font);

    uint64_t memMapEntryCount = bootInfo->memMapSize / bootInfo->memDescSize;

    PageFrameAllocator pageFrameAllocator;
    pageFrameAllocator.ReadEFIMemoryMap(bootInfo->memMap, bootInfo->memMapSize, bootInfo->memDescSize);

    renderer.Print("Kernel start: ");
    renderer.Print(ToStringHex((uint64_t)&kernelStart / 1024));
    renderer.NewLine();

    uint64_t kernelSize = (uint64_t) &kernelEnd - (uint64_t) &kernelStart;
    uint64_t kernelPageCount = (uint64_t) kernelSize / 4096 + 1;

    // UEFI seems to already lock the kernel pages, but just in case.
    pageFrameAllocator.LockPages(&kernelStart, kernelPageCount);

    renderer.Print("Free RAM: ");
    renderer.Print(ToString(pageFrameAllocator.GetFreeRAM() / 1024));
    renderer.Print(" KB");
    renderer.NewLine();

    renderer.Print("Used RAM: ");
    renderer.Print(ToString(pageFrameAllocator.GetUsedRAM() / 1024));
    renderer.Print(" KB");
    renderer.NewLine();

    renderer.Print("Reserved RAM: ");
    renderer.Print(ToString(pageFrameAllocator.GetReservedRAM() / 1024));
    renderer.Print(" KB");
    renderer.NewLine();

    renderer.Print("Total memory: ");
    renderer.Print(ToString(GetTotalMemorySize(bootInfo->memMap, memMapEntryCount, bootInfo->memDescSize) / 1024));
    renderer.Print(" KB");
    renderer.NewLine();

    for (int i = 0; i < 20; ++i)
    {
        void* address = pageFrameAllocator.RequestPage();
        renderer.Print(ToStringHex((uint64_t)address));
        renderer.NewLine();
    }
}