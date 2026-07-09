#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

void kernel_main(void) {
    vga_init();
    idt_init();      // Setup IDT gates (exceptions and IRQs)
    pic_init();      // Remap the PIC and unmask IRQ0, IRQ1, IRQ2
    keyboard_init(); // Register the keyboard callback
    
    // Enable interrupts safely
    __asm__ volatile ("sti");
    
    vga_print_ln("==============================");
    vga_print_ln("  Welcome to TinyOS v0.1");
    vga_print_ln("==============================");
    vga_print_ln("");
    vga_print_ln("Kernel loaded successfully.");
    vga_print_ln("VGA text mode: OK");
    vga_print_ln("");
    vga_print_ln("Keyboard ready. Start typing:");
    vga_print_ln("");

    while (1) {
        char c = keyboard_read(); 
        vga_putchar(c);
    }
}