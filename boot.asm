global _start
extern rkernel

ALIGN_MOD     equ 1<<0
MEM_INFO      equ 1<<1

HEADER_FLAGS  equ ALIGN_MOD | MEM_INFO
HEADER_MAGIC  equ 0x1BADB002
HEADER_CHECK  equ -(HEADER_MAGIC + HEADER_FLAGS)

section .text
align 4

boot_header:
        dd HEADER_MAGIC
        dd HEADER_FLAGS
        dd HEADER_CHECK

_start:
        call rkernel
        cli

halt_loop:
        hlt
        jmp halt_loop

section .bss
        align 4
