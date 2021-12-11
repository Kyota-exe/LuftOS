#include "PageMapIndexer.h"

PageMapIndexer::PageMapIndexer(uint64_t virtAddr)
{
    // Use bit-shifting to retrieve bits that represent indexes
    virtAddr >>= 12;
    pIndex = virtAddr & 0x1ff;
    virtAddr >>= 9;
    ptIndex = virtAddr & 0x1ff;
    virtAddr >>= 9;
    pdIndex = virtAddr & 0x1ff;
    virtAddr >>= 9;
    pdpIndex = virtAddr & 0x1ff;
}