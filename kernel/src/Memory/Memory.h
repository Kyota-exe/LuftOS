#ifndef LUFTOS_MEMORY_H
#define LUFTOS_MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "EfiMemory.h"

uint64_t GetTotalMemorySize(EfiMemoryDescriptor* memMap, uint64_t memMapEntryCount, uint64_t memMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);
void memcpy(void* destination, void* source, size_t num);

#endif
