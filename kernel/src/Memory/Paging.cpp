#include "Paging.h"

void PageDirectoryEntry::SetFlag(PDEFlag flag, bool enable)
{
    uint64_t bitIndexer = (uint64_t)1 << flag;
    if (enable)
    {
        value |= bitIndexer;
    }
    else
    {
        value &= ~bitIndexer;
    }
}

bool PageDirectoryEntry::GetFlag(PDEFlag flag)
{
    uint64_t bitIndexer = (uint64_t)1 << flag;
    return value & bitIndexer;
}

uint64_t PageDirectoryEntry::GetAddress()
{
    return (value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address)
{
    address &= 0x000000ffffffffff;
    value &= 0xfff0000000000fff;
    value |= (address << 12);
}