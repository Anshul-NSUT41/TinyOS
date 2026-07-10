#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

void kernel_main(void) {
    vga_init();
    idt_init();      // Setup IDT gates (exceptions and IRQs)
    pic_init();      // Remap the PIC and unmask IRQ0, IRQ1, IRQ2
    keyboard_init(); // Register the keyboard callback

    vga_init();

/* Banner */
vga_set_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
vga_print_ln("                          ==============================");
vga_print_ln("                             Welcome to TinyOS v0.1");
vga_print_ln("                          ==============================");
vga_print_ln("");

/* Success messages */
vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
vga_print_ln("Kernel loaded successfully.");
vga_print_ln("VGA text mode: OK");
vga_print_ln("IDT Loaded.");
vga_print_ln("PIC Initialized.");
vga_print_ln("Keyboard ready.");
vga_print_ln("");

/* Prompt */
vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
vga_print(" tinyOS > ");
vga_print_ln("");
    
    // Enable interrupts safely
    __asm__ volatile ("sti");

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