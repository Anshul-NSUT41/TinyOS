#include "vga.h"

static uint16_t* VGA_MEMORY  (uint16_t*) 0xB8000;

static size_t vga_row;
static size_t vga_col;
static uint8_t vga_color;

// Pack a foreground + background color into one byte
// Upper 4 bits = background, lower 4 bits = foreground

static uint8_t make_color(vga_color_t fg, vga_color_t bg) {
    return fg | bg << 4;
}

static uint16_t make_vga_entry(char c, uint8_t color) {
    return (uint16_t) c | (uint16_t) color << 8;
}

void vga_set_color(vga_color_t fg, vga_color_t bg) {
    vga_color = make_color(fg, bg);
}

void vga_clear(void){
    uint16_t blank = make_vga_entry(' ' , vga_color);
    for(size_t y = 0 ; y < VGA_HEIGHT * VGA_WIDTH ; y++){
        VGA_MEMORY[y] = blank ;
    }
    vga_row = 0 ;
    vga_col = 0 ;
}

void vga_init() {
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear();
}


