#include "PageTableManager.h"

PageTableManager::PageTableManager(PageTable *pml4Addr) : pml4(pml4Addr) { }

void PageTableManager::MapMemory(void* virtAddr, void* physAddr)
{
    PageMapIndexer pageMapIndexer = PageMapIndexer((uint64_t)virtAddr);

    // Get the entry containing the pointer to the PDP (page table of page tables of page tables of pages)
    PageDirectoryEntry pageDirectoryEntry;
    pageDirectoryEntry = pml4->entries[pageMapIndexer.pdpIndex];

    // If the PDP doesn't exist yet, request a page for it
    PageTable* pdp;
    if (!pageDirectoryEntry.present)
    {
        pdp = (PageTable*)globalPageFrameAllocator.RequestPage();
        memset(pdp, 0, 0x1000);
        pageDirectoryEntry.address = (uint64_t)pdp >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        // Since we got the struct itself instead of a pointer to it, we must set it back
        pml4->entries[pageMapIndexer.pdpIndex] = pageDirectoryEntry;
    }
    else
    {
        pdp = (PageTable*)((uint64_t)pageDirectoryEntry.address << 12);
    }

    // Do the same thing to get the PD (page table of page tables of pages)
    pageDirectoryEntry = pdp->entries[pageMapIndexer.pdIndex];
    PageTable* pd;
    if (!pageDirectoryEntry.present)
    {
        pd = (PageTable*)globalPageFrameAllocator.RequestPage();
        memset(pd, 0, 0x1000);
        pageDirectoryEntry.address = (uint64_t)pd >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pdp->entries[pageMapIndexer.pdIndex] = pageDirectoryEntry;
    }
    else
    {
        pd = (PageTable*)((uint64_t)pageDirectoryEntry.address << 12);
    }

    // Do the same thing to get the PT (page table of pages)
    pageDirectoryEntry = pd->entries[pageMapIndexer.ptIndex];
    PageTable* pt;
    if (!pageDirectoryEntry.present)
    {
        pt = (PageTable*)globalPageFrameAllocator.RequestPage();
        memset(pt, 0, 0x1000);
        pageDirectoryEntry.address = (uint64_t)pt >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pd->entries[pageMapIndexer.ptIndex] = pageDirectoryEntry;
    }
    else
    {
        pt = (PageTable*)((uint64_t)pageDirectoryEntry.address << 12);
    }

    // Get page entry and set the physical address
    pageDirectoryEntry = pt->entries[pageMapIndexer.pIndex];
    pageDirectoryEntry.address = (uint64_t)physAddr >> 12;
    pageDirectoryEntry.present = true;
    pageDirectoryEntry.readWrite = true;
    pt->entries[pageMapIndexer.pIndex] = pageDirectoryEntry;
}