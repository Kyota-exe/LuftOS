#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>

void GetBootFileSystem(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, void** ptr)
{
    // Get loaded image
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage;
    SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loadedImage);

    // Get file system from loaded image (file system that we booted from)
    SystemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ptr);
}

EFI_FILE* LoadFile(EFI_FILE* directory, CHAR16* path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_FILE* loadedFile;

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem;
    GetBootFileSystem(ImageHandle, SystemTable, (void**)&fileSystem);

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

void LoadElf(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE* elfFile, Elf64_Ehdr* elfHeader, UINTN* elfHeaderSize)
{
    Elf64_Phdr* programHeaders;
    // Set ELF file read position to program header offset (location)
    elfFile->SetPosition(elfFile, elfHeader->e_phoff);
    // Allocate memory for program headers
    UINTN programHeadersSize = elfHeader->e_phnum * elfHeader->e_phentsize;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, programHeadersSize, (void**)&programHeaders);
    // Read program headers into memory
    elfFile->Read(elfFile, elfHeaderSize, programHeaders);

    // Handle for each program header
    for (Elf64_Phdr* programHeader = programHeaders;
            (char*)programHeader < (char*)programHeaders + elfHeader->e_phnum * elfHeader->e_phentsize;
            programHeader = (Elf64_Phdr*)((char*)programHeader + elfHeader->e_phentsize))
    {
        if (programHeader->p_type == PT_LOAD)
        {
            // Round up (memory size / page size) in order to get number of pages needed
            int pages = (programHeader->p_memsz + 0x1000 - 1) / 0x1000;
            Elf64_Addr segment = programHeader->p_paddr;
            SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
            // Set ELF file read position to the offset of the program itself
            elfFile->SetPosition(elfFile, programHeader->p_offset);
            UINTN size = programHeader->p_filesz;
            elfFile->Read(elfFile, &size, (void*)segment);
            break;
        }
    }
}

EFI_FILE* LoadKernel(Elf64_Ehdr* elfHeader, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_FILE* kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
    if (kernel == NULL)
    {
        Print(L"Could not load kernel file\n\r");
    }
    else
    {
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
        Print(L"Kernel format is bad \n\r");
    }
    else
    {
        Print(L"Kernel header successfully verified \n\r");
    }

    LoadElf(SystemTable, kernel, elfHeader, &size);

    Print(L"Kernel loaded successfully \n\r");

    return kernel;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    Elf64_Ehdr kernelElfHeader;
    EFI_FILE* kernel = LoadKernel(&kernelElfHeader, ImageHandle, SystemTable);

    int (*KernelStart)() = ((__attribute__((sysv_abi)) int (*)() ) kernelElfHeader.e_entry);
    Print(L"%d\n\r", KernelStart());

    return EFI_SUCCESS;
}
