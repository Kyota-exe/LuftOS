#ifndef LUFTOS_MEMORY_H
#define LUFTOS_MEMORY_H

#include <stdint.h>
#include "EfiMemory.h"

uint64_t GetTotalMemorySize(EfiMemoryDescriptor* memMap, uint64_t memMapEntryCount, uint64_t memMapDescSize);

#endif
