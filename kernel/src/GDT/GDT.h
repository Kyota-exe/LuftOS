#ifndef LUFTOS_GDT_H
#define LUFTOS_GDT_H

#include <stdint.h>

struct GDTDescriptor
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct GDTEntry
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t accessByte;
    uint8_t limit1AndFlags; // 4 bits for limit1 and 4 bits for flags
    uint8_t base2;
} __attribute__((packed));

struct GDT
{
    GDTEntry nullEntry; // 0x00
    GDTEntry kernelCode; // 0x08
    GDTEntry kernelData; // 0x10
    GDTEntry userNull; // 0x18
    GDTEntry userCode; // 0x20
    GDTEntry userData; // 0x28
} __attribute__((packed, aligned(0x1000)));

extern GDT DefaultGDT;

extern "C" void LoadGDT(GDTDescriptor* gdtDescriptor);

#endif
