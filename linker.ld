/* linker.ld */
/* This file tells the linker how to combine object files */

ENTRY(_start)    /* entry point - _start symbol from boot.asm */

SECTIONS {
    /* Base address - kernel loads at 1MB (Multiboot standard) */
    . = 0x00100000;

    /* Multiboot header - must be first so GRUB finds it */
    .multiboot : { *(.multiboot) }

    /* Code section - all executable instructions */
    .text : { *(.text) }

    /* Read-only data - constants and strings */
    .rodata ALIGN(0x1000) : {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data - global variables with values */
    .data ALIGN(0x1000) : {
        *(.data)
        *(.data.*)
    }

    /* Uninitialized data - global variables without values */
    .bss ALIGN(0x1000) : {
        sbss = .;           /* start of BSS */
        *(COMMON)           /* common uninitialized symbols */
        *(.bss)
        *(.bss.*)
        ebss = .;           /* end of BSS */
    }

    /* Discard unwanted sections */
    /DISCARD/ : {
        *(.comment)         /* compiler comments */
        *(.eh_frame)        /* C++ exception info */
        *(.note.gnu.build-id) /* build ID */
    }
}
