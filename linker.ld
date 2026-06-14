ENTRY(_start)

SECTIONS
{
    . = 0x10000;
        .multiboot ALIGN(4) : {
        *(.multiboot)
    }
    .text ALIGN(4096) : {
        *(.text)
    }
     .rodata ALIGN(4096) : {
        *(.rodata)
    }
    /* .data holds initialized global variables */
    .data ALIGN(4096) : {
        *(.data)
    }
    /* .bss holds uninitialized globals and our stack */
    .bss ALIGN(4096) : {
        *(COMMON)
        *(.bss)
    }

}