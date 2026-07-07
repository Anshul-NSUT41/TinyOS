#ifndef PORTS_H
#define PORTS_H

#include "../include/types.h"

//Reads 1 byte from port
static inline uint8_t port_byte_in(uint16_t port){
    uint8_t result ;
    __asm__ volatile ("inb %1 , %0" : "=a"(result) : "Nd"(port));
    return result ;
}

//Writes 1 byte to port
static inline void port_byte_out(uint16_t port , uint8_t data){
    __asm__ volatile ("outb %0 , %1" : : "a"(data) , "Nd"(port));
}

static inline void io_wait(void){
    port_byte_out(0x80, 0); 
}