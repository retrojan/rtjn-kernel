ENTRY(_start)

SECTIONS {
    . = 0x00100000;
    
    .multiboot : { *(.multiboot) } 
    .text : { *(.text) }
    
    .rodata ALIGN(0x1000) : {
        *(.rodata)
        *(.rodata.*)
    }
    
    .data ALIGN(0x1000) : {
        *(.data)
        *(.data.*)
    }
    
    .bss ALIGN(0x1000) : {
        sbss = .;
        *(COMMON)
        *(.bss)
        *(.bss.*)
        ebss = .;
    }
    
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}
