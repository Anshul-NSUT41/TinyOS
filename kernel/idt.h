#ifndef IDT_H
#define IDT_H

#include "../include/types.h"

typedef struct __attribute__((packed)) { 
    uint16_t selector;    
    uint8_t  zero;        
    uint8_t  flags;     
    uint16_t base_high;   
} idt_entry_t;

typedef struct __attribute__((packed)) {
    uint16_t limit;      
    uint32_t base;        
} idt_ptr_t;

void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif