#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>
#include <stdint.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

#define BMP_MAGIC0 'B'
#define BMP_MAGIC1 'M'

typedef struct
{
    void* baseAddress;
    size_t bufferSize;
    uint32_t width;
    uint32_t height;
    uint32_t pixelsPerScanLine;
} Framebuffer;

typedef struct __attribute__((packed))
{
    unsigned char magic[2];
    uint8_t mode;
    uint8_t charSize;
} Psf1Header;

typedef struct
{
    Psf1Header* psf1Header;
    void* glyphBuffer;
} Psf1Font;

typedef struct __attribute__((packed))
{
    unsigned char magic[2];
    uint32_t fileSize;
    uint16_t reserved0;
    uint16_t reserved1;
    uint32_t offset;
} BMPHeader;

typedef struct __attribute__((packed))
{
    uint32_t dibSize;
    uint32_t width; // LONG
    uint32_t height; // LONG
    uint16_t planesCount;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t bitmapSize;
    uint32_t printWidth; // LONG
    uint32_t printHeight; // LONG
    uint32_t colorCountInColorPalette;
    uint32_t importantColors;
    uint32_t redBitMask;
    uint32_t greenBitMask;
    uint32_t blueBitMask;
    uint32_t alphaBitMask;
    uint32_t colorSpace;
    unsigned char endpoints[36];
    uint32_t gammaRed;
    uint32_t gammaGreen;
    uint32_t gammaBlue;
    uint32_t intent;
    uint32_t profileData;
    uint32_t profileSize;
    uint32_t reserved;
} BITMAPV5HEADER;

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t bitmapSize;
    void* bitmapBuffer;
    uint32_t redBitMask;
    uint32_t greenBitMask;
    uint32_t blueBitMask;
    uint32_t alphaBitMask;
} BMPImage;

typedef struct
{
    Framebuffer* framebuffer;
    Psf1Font* psf1Font;
    BMPImage* bmpImage;
    EFI_MEMORY_DESCRIPTOR* memMap;
    UINTN memMapSize;
    UINTN memDescSize;
} BootInfo;


int memcmp(const void* aptr, const void* bptr, size_t n)
{
    const unsigned char* a = aptr;
    const unsigned char* b = bptr;

    for (size_t i = 0; i < n; i++)
    {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }
    return 0;
}

Framebuffer framebuffer;
void InitializeFramebufferStruct(Framebuffer* framebuffer, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop)
{
    framebuffer->baseAddress = (void*)gop->Mode->FrameBufferBase;
    framebuffer->bufferSize = gop->Mode->FrameBufferSize;
    framebuffer->width = gop->Mode->Info->HorizontalResolution;
    framebuffer->height = gop->Mode->Info->VerticalResolution;
    framebuffer->pixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
}

void InitializeGOP(BootInfo* bootInfo)
{
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS status;

    //status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    if (EFI_ERROR(status))
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Unable to locate GOP\n\r");
    }
    else
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN);
        Print(L"GOP located\n\r");
    }

    InitializeFramebufferStruct(&framebuffer, gop);

    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGRAY);
    Print(L"baseAddress: %x\n\rbufferSize: %x\n\rwidth: %d\n\rheight: %d\n\rpixelsPerScanLine: %d\n\r",
          framebuffer.baseAddress,
          framebuffer.bufferSize,
          framebuffer.width,
          framebuffer.height,
          framebuffer.pixelsPerScanLine);

    bootInfo->framebuffer = &framebuffer;
}

void GetBootFileSystem(EFI_HANDLE image, void** ptr)
{
    // Get loaded image
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage;
    BS->HandleProtocol(image, &gEfiLoadedImageProtocolGuid, (void**)&loadedImage);

    // Get file system from loaded image (file system that we booted from)
    BS->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ptr);
}

EFI_FILE* LoadFile(EFI_FILE* directory, CHAR16* path, EFI_HANDLE image)
{
    EFI_FILE* loadedFile;

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem;
    GetBootFileSystem(image, (void**)&fileSystem);

    // Set file system directory to root if directory is NULL
    if (directory == NULL)
    {
        fileSystem->OpenVolume(fileSystem, &directory);
    }

    // Load file with read only permissions
    EFI_STATUS status = directory->Open(directory, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (status != EFI_SUCCESS)
    {
        return 0;
    }
    return loadedFile;
}

Psf1Font* LoadPsf1Font(EFI_FILE* directory, CHAR16* path, EFI_HANDLE image, BootInfo* bootInfo)
{
    // Load font file
    EFI_FILE* font = LoadFile(directory, path, image);
    if (font == NULL) return NULL;

    // Store font header in memory
    Psf1Header* fontHeader;
    UINTN headerSize = sizeof(Psf1Header);
    BS->AllocatePool(EfiLoaderData, headerSize, (void**)&fontHeader);
    font->Read(font, &headerSize, fontHeader);

    // Check if font file has valid header using magic bytes
    if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
    {
        return NULL;
    }

    // Standard glyph count: 256
    // If mode is 1, 512 glyph mode
    UINTN glyphBufferSize = fontHeader->charSize * (fontHeader->mode == 1 ? 512 : 256);

    // Read glyphs into buffer stored in memory
    void* glyphBuffer;
    font->SetPosition(font, headerSize);
    BS->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
    font->Read(font, &glyphBufferSize, glyphBuffer);

    // Store struct with font header and glyph buffer addresses into memory
    Psf1Font* finishedFont;
    BS->AllocatePool(EfiLoaderData, sizeof(Psf1Font), (void**)&finishedFont);
    finishedFont->psf1Header = fontHeader;
    finishedFont->glyphBuffer = glyphBuffer;

    bootInfo->psf1Font = finishedFont;
    return finishedFont;
}

BMPImage* LoadBMPImage(EFI_FILE* directory, CHAR16* path, EFI_HANDLE image, BootInfo* bootInfo)
{
    // Load BMP file
    EFI_FILE* bmpFile = LoadFile(directory, path, image);
    if (bmpFile == NULL)
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"An error occurred while attempting to load BMP file\n\r");
        return NULL;
    }

    // Store BMP header in memory
    BMPHeader* bmpHeader;
    UINTN headerSize = sizeof(BMPHeader);
    BS->AllocatePool(EfiLoaderData, headerSize, (void**)&bmpHeader);
    bmpFile->Read(bmpFile, &headerSize, bmpHeader);

    // Check if BMP file has valid header using magic bytes
    if (bmpHeader->magic[0] != BMP_MAGIC0 || bmpHeader->magic[1] != BMP_MAGIC1)
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"An error occurred while attempting to load BMP file\n\r");
        return NULL;
    }

    UINTN dibSize;
    bmpFile->SetPosition(bmpFile, headerSize);
    UINTN dibSizeSize = 4;
    bmpFile->Read(bmpFile, &dibSizeSize, &dibSize);
    if (dibSize != 124) // BITMAPV5HEADER
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Invalid or unsupported BMP file format\n\r");
        return NULL;
    }

    BITMAPV5HEADER* dibHeader;
    bmpFile->SetPosition(bmpFile, headerSize);
    BS->AllocatePool(EfiLoaderData, dibSize, (void**)&dibHeader);
    bmpFile->Read(bmpFile, &dibSize, dibHeader);

    Print(L"DIB Size: %d\n\r", dibHeader->dibSize);
    Print(L"Width: %d\n\r", dibHeader->width);
    Print(L"Height: %d\n\r", dibHeader->height);

    if (dibHeader->bitsPerPixel != 32)
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Unsupported BPP in BMP file\n\r");
        return NULL;
    }

    if (dibHeader->compressionMethod != 3) // BI_BITFIELDS
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Unsupported BMP file compression format\n\r");
        return NULL;
    }

    void* bitmapBuffer;
    UINTN bitmapSize = dibHeader->bitmapSize;
    bmpFile->SetPosition(bmpFile, bmpHeader->offset);
    BS->AllocatePool(EfiLoaderData, dibHeader->bitmapSize, (void**)&bitmapBuffer);

    Print(L"Reading BMP file contents...\n\r");
    bmpFile->Read(bmpFile, &bitmapSize, bitmapBuffer);
    Print(L"Successfully read BMP File contents into memory.\n\r");

    BMPImage* bmpImage;
    BS->AllocatePool(EfiLoaderData, sizeof(BMPImage), (void**)&bmpImage);

    bmpImage->width = dibHeader->width;
    bmpImage->height = dibHeader->height;
    bmpImage->bitmapSize = dibHeader->bitmapSize;
    bmpImage->bitmapBuffer = bitmapBuffer;
    bmpImage->alphaBitMask = dibHeader->alphaBitMask;
    bmpImage->redBitMask = dibHeader->redBitMask;
    bmpImage->greenBitMask = dibHeader->greenBitMask;
    bmpImage->blueBitMask = dibHeader->blueBitMask;

    Print(L"Red bitmask: %x\n\r", dibHeader->redBitMask);
    // Red 00000000 11111111 00000000 00000000
    // Green 00000000 00000000 11111111 00000000
    // Blue 00000000 00000000 00000000 11111111
    // Alpha 11111111 00000000 00000000 00000000
    Print(L"Green bitmask: %x\n\r", dibHeader->greenBitMask);
    Print(L"Blue bitmask: %x\n\r", dibHeader->blueBitMask);
    Print(L"Alpha bitmask: %x\n\r", dibHeader->alphaBitMask);

    bootInfo->bmpImage = bmpImage;
    return bmpImage;
}

void LoadElf(EFI_FILE* elfFile, Elf64_Ehdr* elfHeader)
{
    Elf64_Phdr* programHeaders;
    // Set ELF file read position to program header offset (location)
    elfFile->SetPosition(elfFile, elfHeader->e_phoff);
    // Allocate memory for program headers
    UINTN programHeadersSize = elfHeader->e_phnum * elfHeader->e_phentsize;
    BS->AllocatePool(EfiLoaderData, programHeadersSize, (void**)&programHeaders);
    // Read program headers into memory
    elfFile->Read(elfFile, &programHeadersSize, programHeaders);

    // Handle for each program header
    for (Elf64_Phdr* programHeader = programHeaders;
            (char*)programHeader < (char*)programHeaders + elfHeader->e_phnum * elfHeader->e_phentsize;
            programHeader = (Elf64_Phdr*)((char*)programHeader + elfHeader->e_phentsize))
    {
        if (programHeader->p_type == PT_LOAD)
        {
            int pages = (programHeader->p_memsz + 0x1000 - 1) / 0x1000;
            Elf64_Addr segment = programHeader->p_paddr;
            BS->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
            // Set ELF file read position to the offset of the program itself
            elfFile->SetPosition(elfFile, programHeader->p_offset);
            UINTN size = programHeader->p_filesz;
            elfFile->Read(elfFile, &size, (void*)segment);
        }
    }
}

EFI_FILE* LoadKernel(Elf64_Ehdr* elfHeader, EFI_HANDLE image)
{
    EFI_FILE* kernel = LoadFile(NULL, L"kernel.elf", image);
    if (kernel == NULL)
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Could not load kernel file\n\r");
    }
    else
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN);
        Print(L"Kernel file loaded successfully\n\r");
    }

    UINTN size = sizeof(Elf64_Ehdr);
    kernel->Read(kernel, &size, elfHeader);

    // Check if ELF header has expected values for kernel file
    if (memcmp(&(elfHeader->e_ident[EI_MAG0]), ELFMAG, SELFMAG) != 0 ||
        elfHeader->e_ident[EI_CLASS] != ELFCLASS64 ||
        elfHeader->e_ident[EI_DATA] != ELFDATA2LSB ||
        elfHeader->e_type != ET_EXEC ||
        elfHeader->e_machine != EM_X86_64 ||
        elfHeader->e_version != EV_CURRENT )
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Kernel format is bad \n\r");
    }
    else
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN);
        Print(L"Kernel header successfully verified \n\r");
    }

    LoadElf(kernel, elfHeader);

    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN);
    Print(L"Kernel loaded successfully \n\r");

    return kernel;
}

void InitializeMemoryMap(BootInfo* bootInfo, UINTN* memMapKey)
{
    EFI_MEMORY_DESCRIPTOR* memMap = NULL;
    UINTN memMapSize, memDescSize;
    UINT32 memDescVersion;

    // Get memory map size
    BS->GetMemoryMap(&memMapSize, memMap, memMapKey, &memDescSize, &memDescVersion);

    // Allocate memory for memory map
    BS->AllocatePool(EfiLoaderData, memMapSize, (void**)&memMap);

    // Read memory map into memory
    BS->GetMemoryMap(&memMapSize, memMap, memMapKey, &memDescSize, &memDescVersion);

    // Boot Info Initialization
    bootInfo->memMap = memMap;
    bootInfo->memMapSize = memMapSize;
    bootInfo->memDescSize = memDescSize;
}

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE* systemTable)
{
    InitializeLib(image, systemTable);

    // Load Kernel ELF
    Elf64_Ehdr kernelElfHeader;
    EFI_FILE* kernel = LoadKernel(&kernelElfHeader, image);

    BootInfo bootInfo;

    // Load PSF1 font
    Psf1Font* font = LoadPsf1Font(NULL, L"zap-light16.psf", image, &bootInfo);
    if (font == NULL)
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Font is not valid or could not be found\n\r");
    }
    else
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN);
        Print(L"Font loaded charSize: %d\n\r", font->psf1Header->charSize);
    }

    // Load BMP desktop background image
    BMPImage* bmpImage = LoadBMPImage(NULL, L"Desktop-Background.bmp", image, &bootInfo);
    if (bmpImage == NULL)
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
        Print(L"Could not load BMP image\n\r");
    }
    else
    {
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN);
        Print(L"BMP image loaded %dx%d\n\r", bmpImage->width, bmpImage->height);
    }

    InitializeGOP(&bootInfo);

    UINTN memMapKey;
    InitializeMemoryMap(&bootInfo, &memMapKey);

    // Create function pointer for kernel ELF entry point
    void (*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*)) kernelElfHeader.e_entry);

    BS->ExitBootServices(image, memMapKey);

    KernelStart(&bootInfo);

    return EFI_SUCCESS;
}
