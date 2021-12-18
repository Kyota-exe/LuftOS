#include "Memory.h"

uint64_t GetTotalMemorySize(EfiMemoryDescriptor* memMap, uint64_t memMapEntryCount, uint64_t memDescSize)
{
    // Total memory size in bytes
    static uint64_t totalMemSize = 0;

    // If total memory size has already been calculated, return it.
    if (totalMemSize > 0) return totalMemSize;

    for (int i = 0; i < memMapEntryCount; ++i)
    {
        EfiMemoryDescriptor* desc = (EfiMemoryDescriptor*)((uint64_t)memMap + (i * memDescSize));
        totalMemSize += desc->pagesCount * 4096;
    }

    return totalMemSize;
}

void memset(void* start, uint8_t value, size_t num)
{
    for (uint64_t i = 0; i < num; ++i)
    {
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}

void memcpy(void* destination, void* source, size_t num)
{
    char* src = (char*)source;
    char* dest = (char*)dest;

    for (size_t i = 0; i < num; ++i)
    {
        dest[i] = src[i];
    }
}