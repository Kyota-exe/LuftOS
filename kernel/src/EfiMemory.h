#ifndef LUFTOS_EFIMEMORY_H
#define LUFTOS_EFIMEMORY_H

#include <stdint.h>

struct EfiMemoryDescriptor
{
    uint32_t type;
    void* physAddr;
    void* virtAddr;
    uint64_t pagesCount;
    uint64_t attributes;
};

extern const char* EFI_MEMORY_TYPES[];

#endif
