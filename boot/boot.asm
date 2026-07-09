bits 32
MULTIBOOT_MAGIC equ 0x1BADB002 
MULTIBOOT_FLAGS equ 0x00000003 
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) 

; ── Multiboot header section ───────────────────────────────────────────────────
section .multiboot
align 4  
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

; We reserve 16KB for the stack
section .bss
align 16 
stack_bottom:
  resb 16384 ; 16KB stack
stack_top:

; ── Kernel Entry Point ───────────────────────────────────────────────────
section .text
global _start 
extern kernel_main

_start:
  ; Explicitly set data segment selectors to kernel data descriptor (0x18)
  mov ax, 0x18
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  mov esp, stack_top 
  call kernel_main 
  
.hang:
   cli  
   hlt
   jmp .hang
