#ifndef LUFTOS_PAGETABLEMANAGER_H
#define LUFTOS_PAGETABLEMANAGER_H

#include "Paging.h"
#include "PageMapIndexer.h"
#include "PageFrameAllocator.h"
#include "Memory.h"

class PageTableManager
{
public:
    void MapMemory(void* virtAddr, void* physAddr);
    PageTableManager(PageTable* pml4Addr);

private:
    PageTable* pml4; // There is only 1 PML4 per process; it is the highest-level page directory
};

#endif
