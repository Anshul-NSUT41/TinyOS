#ifndef IDT_H
#define IDT_H

#include "types.h"

// Structure representing registers pushed onto the stack during an interrupt
typedef struct {
    uint32_t ds;                                     // Data segment selector (pushed manually)
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                       // Pushed by ISR/IRQ stub (pushed manually or CPU error code)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed automatically by CPU
} registers_t;

// Packed structure for an IDT entry
typedef struct __attribute__((packed)) {
    uint16_t base_low;   // Lower 16 bits of the interrupt handler address
    uint16_t selector;   // Code segment selector (GDT selector)
    uint8_t  zero;       // Always 0
    uint8_t  flags;      // Access flags/gate type
    uint16_t base_high;  // Upper 16 bits of the interrupt handler address
} idt_entry_t;

// Packed structure for the IDT pointer (passed to LIDT)
typedef struct __attribute__((packed)) {
    uint16_t limit;      // Size of the IDT array minus 1
    uint32_t base;       // Start address of the IDT array
} idt_ptr_t;

// Function pointer for interrupt service routine handlers
typedef void (*isr_t)(registers_t *);

// Subsystem functions
void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif