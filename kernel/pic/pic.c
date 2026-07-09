#include "pic.h"
#include "ports.h"

void pic_init(void) {
    // ICW1: Start initialization sequence in cascade mode, expecting ICW4
    port_byte_out(PIC1_COMMAND, 0x11);
    io_wait();
    port_byte_out(PIC2_COMMAND, 0x11);
    io_wait();

    // ICW2: Vector offset configuration
    // Remap Master PIC IRQs (0-7) to CPU interrupts 32-39 (0x20-0x27)
    port_byte_out(PIC1_DATA, 0x20);
    io_wait();
    // Remap Slave PIC IRQs (8-15) to CPU interrupts 40-47 (0x28-0x2F)
    port_byte_out(PIC2_DATA, 0x28);
    io_wait();

    // ICW3: Master/Slave cascade connection routing
    // Tell Master PIC that there is a slave PIC connected to IRQ2 (bit 2 = 0x04)
    port_byte_out(PIC1_DATA, 0x04);
    io_wait();
    // Tell Slave PIC its cascade identity (value 2 = 0x02)
    port_byte_out(PIC2_DATA, 0x02);
    io_wait();

    // ICW4: Set PIC to 8086/88 mode (bit 0 = 0x01)
    port_byte_out(PIC1_DATA, 0x01);
    io_wait();
    port_byte_out(PIC2_DATA, 0x01);
    io_wait();

    // OCW1: Interrupt masks (IMR)
    // Master PIC: Enable IRQ0 (timer), IRQ1 (keyboard), and IRQ2 (cascade line).
    // Bits: 0 (IRQ0), 1 (IRQ1), 2 (IRQ2) are cleared (0 = enabled).
    // Mask = 1111 1000 = 0xF8.
    port_byte_out(PIC1_DATA, 0xF8);
    io_wait();
    // Slave PIC: Mask all interrupts (1111 1111 = 0xFF).
    port_byte_out(PIC2_DATA, 0xFF);
    io_wait();
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        port_byte_out(PIC2_COMMAND, PIC_EOI);
    }
    port_byte_out(PIC1_COMMAND, PIC_EOI);
}