
#include "idt.h"
#include "../include/types.h"

// Our IDT has 256 entries — one for each possible interrupt (0–255)
static idt_entry_t idt[256];
static idt_ptr_t   idt_ptr;

// External assembly function that loads the IDT into the CPU
// (We define this in idt_asm.asm)
extern void idt_load(uint32_t);

// Set one entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;         // Lower 16 bits of address
    idt[num].base_high = (base >> 16) & 0xFFFF; // Upper 16 bits of address
    idt[num].selector  = sel;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
}

void idt_init(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base  = (uint32_t)&idt;

    // Zero out all 256 entries first
    // (We can't use memset — no stdlib — so we do it manually)
    uint8_t* p = (uint8_t*)idt;
    for (uint32_t i = 0; i < sizeof(idt); i++) p[i] = 0;

    // Load the IDT into the CPU using the LIDT instruction
    idt_load((uint32_t)&idt_ptr);
}
