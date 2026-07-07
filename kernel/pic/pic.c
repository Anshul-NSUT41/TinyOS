#include "pic.h"
#include "ports.h"

void pic_init(void){
    port_byte_out(PIC1_COMMAND, 0x11);  // 0x11 = init + ICW4 needed
    port_byte_out(PIC2_COMMAND, 0x11);
    io_wait();


    port_byte_out(PIC1_DATA, 0x20);
    port_byte_out(PIC2_DATA, 0x28);
    io_wait();

    port_byte_out(PIC1_DATA, 0x01);
    port_byte_out(PIC2_DATA, 0x01);
    io_wait();

    port_byte_out(PIC1_DATA, 0xFD);
    port_byte_out(PIC2_DATA, 0xFF);  // Mask all slave IRQs
}

void pic_send_eoi(uint8_t irq){
    if(irq >= 8){
        port_byte_out(PIC2_COMMAND, PIC_EOI);  // Send EOI to slave PIC
    }
    port_byte_out(PIC1_COMMAND, PIC_EOI);  // Send EOI to master PIC
}