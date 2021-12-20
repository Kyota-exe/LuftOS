#include "Kernel.h"

// Temporarily for GUIWindow
Framebuffer* framebuffer;
Psf1Font* psf1Font;

void InitializePaging(BootInfo* bootInfo)
{
    // Request a page to store the kernel PLM4 and fill it with 0
    PageTable* plm4 = (PageTable*) globalPageFrameAllocator.RequestPage();
    memset(plm4, 0, 0x1000);

    // Initialize kernel PLM4 so that it is mapped linearly (virt 0x1000 -> phys 0x1000, virt 0x3237 -> phys 0x3237, etc.)
    PageTableManager pageTableManager = PageTableManager(plm4);
    uint64_t memMapEntryCount = bootInfo->memMapSize / bootInfo->memDescSize;
    uint64_t totalMemory = GetTotalMemorySize(bootInfo->memMap, memMapEntryCount, bootInfo->memDescSize);
    for (uint64_t i = 0; i < totalMemory; i += 4096)
    {
        pageTableManager.MapMemory((void*)i, (void*)i);
    }

    uint64_t framebufferBase = (uint64_t)bootInfo->framebuffer->baseAddress;
    uint64_t framebufferSize = (uint64_t)bootInfo->framebuffer->bufferSize;

    // Lock the pages for the framebuffer to prevent it from being overridden
    globalPageFrameAllocator.LockPages((void*)framebufferBase, framebufferSize / 4096 + 1);

    // Make sure the virt address for the framebuffer is identical to its physical address
    for (uint64_t i = framebufferBase; i < framebufferBase + framebufferSize; i += 4096)
    {
        pageTableManager.MapMemory((void*)i, (void*)i);
    }

    // Activate paging
    asm("mov %0, %%cr3" : : "r" (plm4));
}

void InitializeGDT()
{
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GDT) - 1;
    gdtDescriptor.offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
}

extern "C" void _start(BootInfo* bootInfo)
{
    framebuffer = bootInfo->framebuffer;
    psf1Font = bootInfo->psf1Font;

    InitializeGDT();

    // Initialize renderer and page frame allocator
    BasicRenderer basicRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1Font);

    // Initialize global page frame allocator
    globalPageFrameAllocator = PageFrameAllocator();
    globalPageFrameAllocator.ReadEFIMemoryMap(bootInfo->memMap, bootInfo->memMapSize, bootInfo->memDescSize);

    // UEFI seems to already lock the kernel pages, but just in case, lock the kernel pages.
    uint64_t kernelSize = (uint64_t) &kernelEnd - (uint64_t) &kernelStart;
    uint64_t kernelPageCount = (uint64_t) kernelSize / 4096 + 1;
    globalPageFrameAllocator.LockPages(&kernelStart, kernelPageCount);

    InitializePaging(bootInfo);

    // Render desktop background
#pragma region Desktop background
    if (bootInfo->bmpImage->height != framebuffer->height || bootInfo->bmpImage->width != framebuffer->width)
    {
        uint32_t previousColour = basicRenderer.colour;
        basicRenderer.colour = 0xffff0000;
        basicRenderer.Print("BMP image is not the same resolution as the screen resolution!");
        basicRenderer.NewLine();
        basicRenderer.colour = previousColour;
    }

    // Bottom-up rendering
    for (unsigned int y = 0; y < bootInfo->bmpImage->height; ++y)
    {
        for (unsigned int x = 0; x < bootInfo->bmpImage->width; ++x)
        {
            unsigned int* framebufferPtr = framebuffer->baseAddress + bootInfo->bmpImage->width * y + x;
            unsigned int* pixPtr = bootInfo->bmpImage->bitmapBuffer + (bootInfo->bmpImage->height - 1 - y) * bootInfo->bmpImage->width + x;
            *framebufferPtr = *pixPtr;
        }
    }
#pragma endregion

    //renderer.Print("Kernel initialized successfully.");
    //renderer.NewLine();
    basicRenderer.Print("LuftOS");

    GraphicsRenderer graphicsRenderer = GraphicsRenderer(bootInfo->framebuffer);
    GUIRenderer guiRenderer = GUIRenderer(&graphicsRenderer);

    unsigned int windowWidth = 800;
    unsigned int windowHeight = 500;
    guiRenderer.NewWindow(bootInfo->framebuffer->width / 2 - (windowWidth / 2),
                          bootInfo->framebuffer->height / 2 - (windowHeight / 2),
                          windowWidth, windowHeight);

    while (true);
}