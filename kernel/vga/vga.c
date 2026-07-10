#include "vga.h"

static uint16_t* VGA_MEMORY = (uint16_t*) 0xB8000;

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

static void vga_scroll(void){
    for(size_t row = 1 ; row < VGA_HEIGHT ; row++){
        for(size_t col = 0 ; col < VGA_WIDTH ;  col++){
            size_t prev = (row -1)*VGA_WIDTH + col ;
            size_t curr = row*VGA_WIDTH + col ;
            VGA_MEMORY[prev] = VGA_MEMORY[curr];
        }
    }

    uint16_t blank = make_vga_entry(' ',vga_color);
    for(size_t col = 0 ; col < VGA_WIDTH ; col++){
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
    }
    vga_row = VGA_HEIGHT - 1;
}

void vga_putchar(char c){
    if(c == '\n'){
        vga_col = 0 ;
        if (++vga_row == VGA_HEIGHT) vga_scroll();
        return;
    }
    if(c == '\r'){
        vga_col = 0 ;
        return;
    }   
    if(c == '\b'){
        if(vga_col > 0) vga_col--;
        VGA_MEMORY[vga_row * VGA_WIDTH + vga_col] = make_vga_entry(' ', vga_color);
        return;
    }
    VGA_MEMORY[vga_row * VGA_WIDTH + vga_col] = make_vga_entry(c, vga_color);
    if(++vga_col == VGA_WIDTH){
        vga_col = 0 ;
        if(++vga_row == VGA_HEIGHT) vga_scroll();
    }

}

void vga_print(const char* str) {
    while (*str) vga_putchar(*str++);
}

void vga_print_ln(const char* str) {
    vga_print(str);
    vga_putchar('\n');
}
