#ifndef LUFTOS_PAGEFRAMEALLOCATOR_H
#define LUFTOS_PAGEFRAMEALLOCATOR_H

#include <stdint.h>
#include "EfiMemory.h"
#include "Memory.h"
#include "../Utilities/Bitmap.h"

class PageFrameAllocator
{
public:
    void* RequestPage();
    void ReadEFIMemoryMap(EfiMemoryDescriptor* memMap, size_t memMapSize, size_t memMapDescSize);
    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();
    void FreePage(void* address);
    void FreePages(void* address, uint64_t pageCount);
    void LockPage(void* address);
    void LockPages(void* address, uint64_t pageCount);

    Bitmap pageBitmap;

private:
    void InitBitmap(size_t bitmapSize, void* bufferAddr);
    void ReservePage(void* address);
    void ReservePages(void* address, uint64_t pageCount);
    void UnreservePage(void* address);
    void UnreservePages(void* address, uint64_t pageCount);
};

extern PageFrameAllocator globalPageFrameAllocator;

#endif
