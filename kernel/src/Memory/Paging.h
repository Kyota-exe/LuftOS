#ifndef LUFTOS_PAGING_H
#define LUFTOS_PAGING_H

#include <stdint.h>

enum PDEFlag
{
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63
};

struct PageDirectoryEntry
{
    uint64_t value;
    void SetFlag(PDEFlag flag, bool enable);
    bool GetFlag(PDEFlag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
}; // 64 bits in total

struct PageTable
{
    PageDirectoryEntry entries[512];
}__attribute__((aligned(0x1000)));

#endif
