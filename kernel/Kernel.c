#include "vga.h"

void kernel_main(void) {
    vga_init();

    vga_print_ln("==============================");
    vga_print_ln("  Welcome to TinyOS v0.1");
    vga_print_ln("==============================");
    vga_print_ln("");
    vga_print_ln("Kernel loaded successfully.");
    vga_print_ln("VGA text mode: OK");
    vga_print_ln("");
    while (1) {}
}