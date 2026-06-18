#ifndef PORTS_H
#define PORTS_H

#include "../include/types.h"

//Reads 1 byte from port
static inline uint8_t port_byte_in(uint16_t port){
    uint8_t result ;
    __asm__ volatile ("inb %1 , %0" : "=a"(result) : "Nd"(port));
    return result ;
}