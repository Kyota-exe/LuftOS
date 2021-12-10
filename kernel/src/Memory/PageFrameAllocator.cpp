#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool initialized = false;

void PageFrameAllocator::ReadEFIMemoryMap(EfiMemoryDescriptor* memMap, size_t memMapSize, size_t memMapDescSize)
{
    if (initialized) return;
    initialized = true;

    uint64_t memMapEntryCount = memMapSize / memMapDescSize;

    // Find the largest free memory segment to store page bitmap in
    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    for (int i = 0; i < memMapEntryCount; ++i)
    {
        EfiMemoryDescriptor* desc = (EfiMemoryDescriptor*)((uint64_t) memMap + (i * memMapDescSize));
        if (desc->type == 7) // EfiConventionalMemory
        {
            if (desc->pagesCount * 4096 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->pagesCount * 4096;
            }
        }
    }

    // Set initial free memory and determine page bitmap size
    uint64_t memorySize = GetTotalMemorySize(memMap, memMapEntryCount, memMapDescSize);
    freeMemory = memorySize;
    uint64_t pageBitmapSize = memorySize / 4096 / 8 + 1;

    InitBitmap(pageBitmapSize, largestFreeMemSeg);

    // Lock pages of bitmap
    LockPages(&pageBitmap, pageBitmap.size / 4096 + 1);

    // Reserve pages of unusable/reserved memory
    for (int i = 0; i < memMapEntryCount; ++i)
    {
        EfiMemoryDescriptor* desc = (EfiMemoryDescriptor*)((uint64_t) memMap + (i * memMapDescSize));
        if (desc->type != 7) // Not EfiConventionalMemory, therefore unusable/reserved
        {
            ReservePages(desc->physAddr, desc->pagesCount);
        }
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddr)
{
    pageBitmap.size = bitmapSize;
    pageBitmap.buffer = (uint8_t*) bufferAddr;
    for (int i = 0; i < bitmapSize; ++i)
    {
        *(uint8_t*)(pageBitmap.buffer + i) = 0;
    }
}

void* PageFrameAllocator::RequestPage()
{
    // Search from beginning of RAM until free page is found
    for (uint64_t i = 0; i < pageBitmap.size * 8; ++i)
    {
        if (pageBitmap[i]) continue;
        LockPage((void*)(i * 4096));
        return (void*)(i * 4096);
    }

    return NULL; // No more free pages left
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (!pageBitmap[index]) return;
    pageBitmap.Set(index, false);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void PageFrameAllocator::LockPage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (pageBitmap[index]) return;
    pageBitmap.Set(index, true);
    freeMemory -= 4096;
    usedMemory += 4096;
}

void PageFrameAllocator::UnreservePage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (!pageBitmap[index]) return;
    pageBitmap.Set(index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (pageBitmap[index]) return;
    pageBitmap.Set(index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
    {
        FreePage((void*)((uint64_t)address + i * 4096));
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
    {
        LockPage((void*)((uint64_t)address + i * 4096));
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
    {
        ReservePage((void*)((uint64_t)address + i * 4096));
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
    {
        UnreservePage((void*)((uint64_t)address + i * 4096));
    }
}

uint64_t PageFrameAllocator::GetFreeRAM() { return freeMemory; }
uint64_t PageFrameAllocator::GetUsedRAM() { return usedMemory; }
uint64_t PageFrameAllocator::GetReservedRAM() { return reservedMemory; }