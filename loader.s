global loader                               ; the entry symbol for ELF
extern sum_of_three
extern kmain

MAGIC_NUMBER    equ 0x1BADB002              ; define the magic number constant
FLAGS           equ 0x0                     ; multiboot flags
CHECKSUM        equ -MAGIC_NUMBER           ; calculate the CHECKSUM
                                            ; (magic number + checksum + flag should equal 0)
KERNEL_STACK    equ 4096                    ; kernel stack size = 4096 bytes

section .text:                              ; start of the code section
align 4                                     ; the code must be 4 byte aligned
    dd MAGIC_NUMBER                         ; write the magic number to the machine code,
    dd FLAGS                                ; the flags,
    dd CHECKSUM                             ; and the checksum

section .bss:                               ; start of the bss section
align 4
kernel_stack:                               ; label of beginning of memory
    resb KERNEL_STACK                       ; reserve stack for the kernel

loader:                                     ; the loader label (entry point in linker script)
    mov eax, 0xCAFEBABE                     ; place the number 0xCAFEBABE in the register eax
    mov esp, kernel_stack + KERNEL_STACK - 4; esp points to the end of stack (grows backwards)
    
    ;push dword 3
    ;push dword 2
    ;push dword 1
    ;call sum_of_three
    call kmain

.loop:
    jmp .loop