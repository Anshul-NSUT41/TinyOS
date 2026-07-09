bits 32
section .text

global idt_load ; 

; void idt_load(uint32_t idt_ptr_address);
; The argument arrives on stack at [esp + 4]

idt_load:
    mov eax, [esp + 4] ; Load the address of the IDT pointer into EAX
    lidt [eax]         ; Load the IDT using the lidt instruction
    ret                ; Return from the function






