#ifndef LUFTOS_KERNEL_H
#define LUFTOS_KERNEL_H

#include "BasicRenderer.h"
#include "StringConversion.h"
#include "Memory/EfiMemory.h"
#include "Utilities/Bitmap.h"
#include "Memory/PageFrameAllocator.h"
#include "Memory/Paging.h"
#include "Memory/PageTableManager.h"
#include "GDT/GDT.h"
#include "BMPImageFile.h"
#include "GUI/GUIRenderer.h"

struct BootInfo
{
    Framebuffer* framebuffer;
    Psf1Font* psf1Font;
    BMPImage* bmpImage;
    EfiMemoryDescriptor* memMap;
    uint64_t memMapSize;
    uint64_t memDescSize;
};

extern uint64_t kernelStart;
extern uint64_t kernelEnd;

#endif
