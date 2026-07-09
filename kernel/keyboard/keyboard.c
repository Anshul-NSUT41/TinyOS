#include "keyboard.h"
#include "idt.h"
#include "pic.h"
#include "ports.h"
#include "vga.h"

// ── Scancode → ASCII table ────────────────────────────────────────────────

static const char scancode_to_ascii[] = {
    0,    // 0x00 - no key
    0,    // 0x01 - Escape
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=',
    '\b', // 0x0E - Backspace
    '\t', // 0x0F - Tab
    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n', // 0x1C - Enter
    0,    // 0x1D - Left Ctrl
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,    // 0x2A - Left Shift
    '\\',
    'z','x','c','v','b','n','m',',','.','/',
    0,    // 0x36 - Right Shift
    '*',
    0,    // 0x38 - Left Alt
    ' ',  // 0x39 - Space
};

static const char scancode_to_ascii_shift[] = {
    0,  0,
    '!','@','#','$','%','^','&','*','(',')',
    '_','+',
    '\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}',
    '\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|',
    'Z','X','C','V','B','N','M','<','>','?',
    0, '*', 0, ' ',
};

#define KEY_BUFFER_SIZE 256
static char key_buffer[KEY_BUFFER_SIZE];
static volatile uint8_t buf_head = 0;  // next write position
static volatile uint8_t buf_tail = 0;  // next read position

static void keyboard_handler_impl(void) {
    uint8_t scancode = port_byte_in(0x60);  
    // Bit 7 of scancode = key release (1) vs key press (0)
    uint8_t released = scancode & 0x80;
    uint8_t key      = scancode & 0x7F;  
    if (key == 0x2A || key == 0x36) {    
        shift_pressed = !released;
        pic_send_eoi(1);
        return;
    }
    if (released) {
        pic_send_eoi(1);
        return;
    }
    char c = 0;
    if (key < SCANCODE_TABLE_SIZE) {
        c = shift_pressed ? scancode_to_ascii_shift[key]
                          : scancode_to_ascii[key];
    }
    if (c != 0) {
        uint8_t next = (buf_head + 1) % KEY_BUFFER_SIZE;
        if (next != buf_tail) { 
            key_buffer[buf_head] = c;
            buf_head = next;
        }
    }
    pic_send_eoi(1); 
}

// We use a GCC naked function + inline assembly for precise control.
void __attribute__((naked)) keyboard_irq_handler(void) {
    __asm__ volatile (
        "pusha\n"                    // Push all general-purpose registers
        "call keyboard_handler_impl\n"
        "popa\n"                     // Restore all registers
        "iret\n"                     // Return from interrupt
    );
}

void keyboard_init(void) {
    idt_set_gate(33, (uint32_t)keyboard_irq_handler, 0x08, 0x8E);
}

char keyboard_getchar(void) {
    if (buf_head == buf_tail) return 0; 
    char c = key_buffer[buf_tail];
    buf_tail = (buf_tail + 1) % KEY_BUFFER_SIZE;
    return c;
}

char keyboard_read(void) {
    char c;
    while ((c = keyboard_getchar()) == 0) {
        __asm__ volatile ("hlt");  // Sleep until next interrupt (saves CPU)
    }
    return c;
}
