[BITS 64]

LoadGDT:
    lgdt [rdi]

    ; Move all segment registers to kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq

global LoadGDT