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

}