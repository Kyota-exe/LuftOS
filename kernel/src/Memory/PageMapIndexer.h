#ifndef LUFTOS_PAGEMAPINDEXER_H
#define LUFTOS_PAGEMAPINDEXER_H

#include <stdint.h>

class PageMapIndexer
{
public:
    PageMapIndexer(uint64_t virtAddr);
    uint64_t pdpIndex;
    uint64_t pdIndex;
    uint64_t ptIndex;
    uint64_t pIndex;
};

#endif
