#include "BasicRenderer.h"
#include "StringConversion.h"
#include "Memory/EfiMemory.h"
#include "Bitmap.h"
#include "Memory/PageFrameAllocator.h"
#include "Memory/Paging.h"
#include "Memory/PageTableManager.h"

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

void InitializePaging(BootInfo* bootInfo)
{
    PageTable* plm4 = (PageTable*) globalPageFrameAllocator.RequestPage();
    memset(plm4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(plm4);
    uint64_t memMapEntryCount = bootInfo->memMapSize / bootInfo->memDescSize;
    uint64_t totalMemory = GetTotalMemorySize(bootInfo->memMap, memMapEntryCount, bootInfo->memDescSize);
    for (uint64_t i = 0; i < totalMemory; i += 4096)
    {
        pageTableManager.MapMemory((void *) i, (void *) i);
    }

    uint64_t framebufferBase = (uint64_t)bootInfo->framebuffer->baseAddress;
    uint64_t framebufferSize = (uint64_t)bootInfo->framebuffer->bufferSize;
    globalPageFrameAllocator.LockPages((void*)framebufferBase, framebufferSize / 4096 + 1);
    for (uint64_t i = framebufferBase; i < framebufferBase + framebufferSize; i += 4096)
    {
        pageTableManager.MapMemory((void*)i, (void*)i);
    }

    asm("mov %0, %%cr3" : : "r" (plm4));
}

extern "C" void _start(BootInfo* bootInfo)
{
    // Initialize renderer and page frame allocator
    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1Font);

    // Initialize global page frame allocator
    globalPageFrameAllocator = PageFrameAllocator();
    globalPageFrameAllocator.ReadEFIMemoryMap(bootInfo->memMap, bootInfo->memMapSize, bootInfo->memDescSize);

    // UEFI seems to already lock the kernel pages, but just in case, lock the kernel pages.
    uint64_t kernelSize = (uint64_t) &kernelEnd - (uint64_t) &kernelStart;
    uint64_t kernelPageCount = (uint64_t) kernelSize / 4096 + 1;
    globalPageFrameAllocator.LockPages(&kernelStart, kernelPageCount);

    InitializePaging(bootInfo);

    // Clear framebuffer
    memset(bootInfo->framebuffer->baseAddress, 0, bootInfo->framebuffer->bufferSize);

    renderer.Print("Kernel successfully loaded.");
}