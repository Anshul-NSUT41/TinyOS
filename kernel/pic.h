// Programmable Interrupt Controller (PIC)  manages hardware IRQs

#ifndef PIC_H
#define PIC_H

#include "../include/types.h"


// After remapping:
//   IRQ0  (timer)    → interrupt 32
//   IRQ1  (keyboard) → interrupt 33
//   IRQ2–7           → interrupts 34–39
//   IRQ8–15          → interrupts 40–47

