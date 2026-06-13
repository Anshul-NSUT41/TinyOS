bits 32
MULTIBOOT_MAGIC equ 0x1BADB002 ;
MULTIBOOT_FLAGS equ 0x00000003 ;
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) ;

; ── Multiboot header section ───────────────────────────────────────────────────
section .multiboot
align 4  ;
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

// we reserve 16KB for the bootloader, so that it can load the kernel at 0x100000
