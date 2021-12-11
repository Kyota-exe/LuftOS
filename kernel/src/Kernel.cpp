#include "BasicRenderer.h"
#include "StringConversion.h"
#include "Memory/EfiMemory.h"
#include "Bitmap.h"
#include "Memory/PageFrameAllocator.h"
#include "Memory/PageMapIndexer.h"

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
    // Initialize renderer and page frame allocator
    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1Font);
    PageFrameAllocator pageFrameAllocator;
    pageFrameAllocator.ReadEFIMemoryMap(bootInfo->memMap, bootInfo->memMapSize, bootInfo->memDescSize);

    // UEFI seems to already lock the kernel pages, but just in case, lock the kernel pages.
    uint64_t kernelSize = (uint64_t) &kernelEnd - (uint64_t) &kernelStart;
    uint64_t kernelPageCount = (uint64_t) kernelSize / 4096 + 1;
    pageFrameAllocator.LockPages(&kernelStart, kernelPageCount);

    PageMapIndexer pageIndexer = PageMapIndexer(0x1000);

    renderer.Print("Kernel successfully loaded.");
}