bits 32

section .text

global idt_load
extern interrupt_handler

; ------------------------------------------------------------------------------
; Load the Interrupt Descriptor Table (IDT)
; void idt_load(uint32_t idt_ptr_addr)
; ------------------------------------------------------------------------------
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

; ------------------------------------------------------------------------------
; Common interrupt handler stub
; ------------------------------------------------------------------------------
isr_common_stub:
    pusha                    ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds               ; Get lower 16 bits of ds descriptor
    push eax                 ; Push ds as 32-bit dword

    mov ax, 0x18             ; Load kernel data segment descriptor (0x18)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                 ; Push esp (points to registers_t on the stack)
    call interrupt_handler
    add esp, 4               ; Clean up the pushed esp

    pop eax                  ; Restore original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Restore pushed general purpose registers
    add esp, 8               ; Clean up pushed error code and interrupt number
    iretd                    ; Return from interrupt (restores CS, EIP, EFLAGS, etc.)

; ------------------------------------------------------------------------------
; Macros for ISR and IRQ definitions
; ------------------------------------------------------------------------------

; For CPU exceptions that do NOT push an error code
%macro ISR_NOERR 1
global isr%1
isr%1:
    push 0                  ; Push dummy error code 0
    push %1                 ; Push interrupt vector number
    jmp isr_common_stub
%endmacro

; For CPU exceptions that DO push an error code
%macro ISR_ERR 1
global isr%1
isr%1:
    push %1                 ; Push interrupt vector number (error code already pushed by CPU)
    jmp isr_common_stub
%endmacro

; For hardware interrupts (IRQs)
%macro IRQ 2
global irq%1
irq%1:
    push 0                  ; Push dummy error code 0
    push %2                 ; Push interrupt vector number
    jmp isr_common_stub
%endmacro

; ------------------------------------------------------------------------------
; Define 32 CPU Exception Stubs (0-31)
; ------------------------------------------------------------------------------
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR   21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_ERR   30
ISR_NOERR 31

; ------------------------------------------------------------------------------
; Define 16 Hardware IRQ Stubs (32-47)
; ------------------------------------------------------------------------------
IRQ 0, 32                  ; IRQ0 (Timer)
IRQ 1, 33                  ; IRQ1 (Keyboard)
IRQ 2, 34                  ; IRQ2 (Cascade)
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
