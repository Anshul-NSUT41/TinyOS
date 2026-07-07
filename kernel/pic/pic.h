// Programmable Interrupt Controller (PIC)  manages hardware IRQs

#ifndef PIC_H
#define PIC_H

#include "../include/types.h"


// After remapping:
//   IRQ0  (timer)    → interrupt 32
//   IRQ1  (keyboard) → interrupt 33
//   IRQ2–7           → interrupts 34–39
//   IRQ8–15          → interrupts 40–47

#define PIC1_COMMAND 0x20   // Master PIC command port
#define PIC1_DATA    0x21   // Master PIC data port
#define PIC2_COMMAND 0xA0   // Slave PIC command port
#define PIC2_DATA    0xA1   // Slave PIC data port
#define PIC_EOI      0x20   // End-of-interrupt command

void pic_init(void);
void pic_send_eoi(uint8_t irq);

#endif