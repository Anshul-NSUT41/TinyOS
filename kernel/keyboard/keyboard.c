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