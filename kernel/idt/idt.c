#include "idt.h"
#include "pic.h"
#include "vga.h"

// IDT array and pointer
static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;

// Array of registered interrupt handlers
static isr_t interrupt_handlers[256];

// Assembly load function
extern void idt_load(uint32_t ptr);

// Assembly ISR declarations (CPU Exceptions)
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

// Assembly IRQ declarations (Hardware Interrupts)
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// CPU Exception messages
static const char *exception_messages[32] = {
    "Division By Zero (#DE)",
    "Debug (#DB)",
    "Non-Maskable Interrupt",
    "Breakpoint (#BP)",
    "Overflow (#OF)",
    "Bound Range Exceeded (#BR)",
    "Invalid Opcode (#UD)",
    "Device Not Available (#NM)",
    "Double Fault (#DF)",
    "Coprocessor Segment Overrun",
    "Invalid TSS (#TS)",
    "Segment Not Present (#NP)",
    "Stack-Segment Fault (#SS)",
    "General Protection Fault (#GP)",
    "Page Fault (#PF)",
    "Reserved",
    "x87 Floating-Point Exception (#MF)",
    "Alignment Check (#AC)",
    "Machine Check (#MC)",
    "SIMD Floating-Point Exception (#XM)",
    "Virtualization Exception (#VE)",
    "Control Protection Exception (#CP)",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception (#SX)",
    "Reserved"
};

// Helper function to print hex values to VGA console
static void print_hex(uint32_t val) {
    char hex_chars[] = "0123456789ABCDEF";
    vga_print("0x");
    for (int i = 28; i >= 0; i -= 4) {
        vga_putchar(hex_chars[(val >> i) & 0xF]);
    }
}

// Visual spinner on PIT timer ticks to verify IRQ0 is firing
static volatile uint32_t timer_ticks = 0;
static void default_timer_handler(registers_t *regs) {
    (void)regs;
    timer_ticks++;
    
    char spinner[] = {'|', '/', '-', '\\'};
    uint16_t* vga_mem = (uint16_t*)0xB8000;
    // Write spinner to top-right corner of VGA screen (row 0, col 79)
    vga_mem[79] = (vga_mem[79] & 0xFF00) | spinner[timer_ticks % 4];
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].selector  = selector;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint32_t)&idt;

    // Clear IDT
    for (int i = 0; i < 256; i++) {
        idt[i].base_low  = 0;
        idt[i].selector  = 0;
        idt[i].zero      = 0;
        idt[i].flags     = 0;
        idt[i].base_high = 0;
        interrupt_handlers[i] = 0;
    }

    // Set gates for all 32 CPU Exceptions (0-31)
    // Selector 0x10 (kernel code selector in GRUB), Flags 0x8E (present, ring 0, 32-bit interrupt gate)
    idt_set_gate(0, (uint32_t)isr0, 0x10, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x10, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x10, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x10, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x10, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x10, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x10, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x10, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x10, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x10, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x10, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x10, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x10, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x10, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x10, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x10, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x10, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x10, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x10, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x10, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x10, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x10, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x10, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x10, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x10, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x10, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x10, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x10, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x10, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x10, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x10, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x10, 0x8E);

    // Set gates for all 16 Hardware IRQs (32-47)
    // PIC Master IRQs
    idt_set_gate(32, (uint32_t)irq0, 0x10, 0x8E); // IRQ0 Timer
    idt_set_gate(33, (uint32_t)irq1, 0x10, 0x8E); // IRQ1 Keyboard
    idt_set_gate(34, (uint32_t)irq2, 0x10, 0x8E); // IRQ2 Cascade
    idt_set_gate(35, (uint32_t)irq3, 0x10, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x10, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x10, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x10, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x10, 0x8E);
    // PIC Slave IRQs
    idt_set_gate(40, (uint32_t)irq8,  0x10, 0x8E);
    idt_set_gate(41, (uint32_t)irq9,  0x10, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x10, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x10, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x10, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x10, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x10, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x10, 0x8E);

    // Load IDT pointer register
    idt_load((uint32_t)&idt_ptr);

    // Register a default timer handler to IRQ0
    register_interrupt_handler(32, default_timer_handler);
}

// Unified interrupt dispatcher in C
void interrupt_handler(registers_t *regs) {
    if (regs->int_no >= 32 && regs->int_no < 48) {
        // Hardware IRQ Interrupt
        uint8_t irq = regs->int_no - 32;
        isr_t handler = interrupt_handlers[regs->int_no];
        if (handler) {
            handler(regs);
        }
        // Send End-Of-Interrupt (EOI) to PIC(s)
        pic_send_eoi(irq);
    } else if (regs->int_no < 32) {
        // CPU Exception Interrupt
        isr_t handler = interrupt_handlers[regs->int_no];
        if (handler) {
            handler(regs);
        } else {
            // Unhandled CPU exception: Panic and show register dump
            vga_clear();
            vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
            vga_clear(); // Fill with blue background
            
            vga_print_ln("================================================================================");
            vga_print("  !!! KERNEL PANIC: UNHANDLED EXCEPTION: ");
            vga_print_ln(exception_messages[regs->int_no]);
            vga_print_ln("================================================================================");
            vga_print_ln("");
            vga_print("  Exception vector: "); print_hex(regs->int_no); vga_print_ln("");
            vga_print("  Error code:       "); print_hex(regs->err_code); vga_print_ln("");
            vga_print("  EIP:              "); print_hex(regs->eip); vga_print_ln("");
            vga_print("  CS:               "); print_hex(regs->cs); vga_print_ln("");
            vga_print("  EFLAGS:           "); print_hex(regs->eflags); vga_print_ln("");
            vga_print("  DS:               "); print_hex(regs->ds); vga_print_ln("");
            vga_print_ln("");
            vga_print_ln("  Register Dump (pusha):");
            vga_print("  EAX: "); print_hex(regs->eax); vga_print("   EBX: "); print_hex(regs->ebx); vga_print_ln("");
            vga_print("  ECX: "); print_hex(regs->ecx); vga_print("   EDX: "); print_hex(regs->edx); vga_print_ln("");
            vga_print("  ESI: "); print_hex(regs->esi); vga_print("   EDI: "); print_hex(regs->edi); vga_print_ln("");
            vga_print("  EBP: "); print_hex(regs->ebp); vga_print("   ESP: "); print_hex(regs->esp); vga_print_ln("");
            vga_print_ln("");
            vga_print_ln("  System Halted. Please restart.");
            
            // Loop forever with interrupts disabled
            while (1) {
                __asm__ volatile ("cli; hlt");
            }
        }
    }
}