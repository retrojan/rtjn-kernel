; boot.asm
; https://github.com/retrojan/rtjn-kernel

global _start      ; make _start visible to the linker (entry point)
extern rkernel     ; function defined in kernel.c (C code)

; Multiboot header constants
ALIGN_MOD     equ 1<<0          ; align boot modules to 4KB page boundaries
MEM_INFO      equ 1<<1          ; request memory map from bootloader
HEADER_FLAGS  equ ALIGN_MOD | MEM_INFO  ; combine flags (1 | 2 = 3)
HEADER_MAGIC  equ 0x1BADB002    ; magic number - GRUB checks this
HEADER_CHECK  equ -(HEADER_MAGIC + HEADER_FLAGS) ; checksum: magic+flags+check = 0

section .text
align 4                          ; required by Multiboot standard

; Multiboot header - GRUB reads this first (must be in first 8KB)
boot_header:
        dd HEADER_MAGIC         ; 0x1BADB002
        dd HEADER_FLAGS         ; 3 (ALIGN_MOD | MEM_INFO)
        dd HEADER_CHECK         ; -(0x1BADB002 + 3)

; Entry point - GRUB jumps here after loading the kernel
_start:
        call rkernel             ; jump to C kernel function (rkernel)
        cli                      ; disable interrupts (safety)

; Safety loop - if rkernel returns, halt forever
halt_loop:
        hlt                      ; stop CPU until next interrupt
        jmp halt_loop            ; if woken up, halt again

section .bss
        align 4                  ; align BSS section to 4 bytes
