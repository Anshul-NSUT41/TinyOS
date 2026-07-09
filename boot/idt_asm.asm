bits 32
section .text

global idt_load ; 

; void idt_load(uint32_t idt_ptr_address);
; The argument arrives on stack at [esp + 4]

idt_load:
    mov eax, [esp + 4] ; Load the address of the IDT pointer into EAX
    lidt [eax]         ; Load the IDT using the lidt instruction
    ret                ; Return from the function






    /*
    cat > ~/tinyos/Makefile << 'EOF'
# ── TinyOS Makefile ───────────────────────────────────────────────────────
# Targets:
#   make          → build the ISO
#   make run      → build and run in QEMU
#   make debug    → build and run with GDB server on port 1234
#   make clean    → delete all build output

# ── Tools ─────────────────────────────────────────────────────────────────
CC     = gcc
AS     = nasm
LD     = ld
GRUB   = grub-mkrescue

# ── Compiler flags ────────────────────────────────────────────────────────
# -m32              : compile for 32-bit x86 (our kernel is 32-bit)
# -ffreestanding    : don't assume a standard library exists
# -fno-builtin      : don't use GCC's built-in replacements for memcpy etc.
# -fno-stack-protector : no stack canaries (requires OS support we don't have)
# -nostdlib         : don't link against the C standard library
# -Wall -Wextra     : enable all warnings
# -Iinclude         : look for headers in the include/ directory
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-stack-protector \
         -nostdlib -Wall -Wextra -Iinclude

# ── Linker flags ──────────────────────────────────────────────────────────
# -m elf_i386       : produce a 32-bit ELF binary
# -T linker.ld      : use our custom memory layout
LDFLAGS = -m elf_i386 -T linker.ld --oformat=elf32-i386

# ── Source files → object files ───────────────────────────────────────────
C_SRCS   = kernel/kernel.c kernel/vga.c
ASM_SRCS = boot/boot.asm

C_OBJS   = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.asm=.o)
ALL_OBJS = $(ASM_OBJS) $(C_OBJS)

# ── Build targets ─────────────────────────────────────────────────────────
.PHONY: all run debug clean

all: tinyos.iso

# Step 1: Compile each .c file into a .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Step 2: Assemble the .asm file into a .o object file
# -f elf32: output 32-bit ELF format (same format as our .c objects)
boot/boot.o: boot/boot.asm
	$(AS) -f elf32 $< -o $@

# Step 3: Link all object files into a single ELF kernel binary
kernel.elf: $(ALL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Step 4: Create a bootable ISO image using GRUB
# grub-mkrescue creates a CD/USB image with GRUB installed as the bootloader
tinyos.iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg   iso/boot/grub/grub.cfg
	$(GRUB) -o tinyos.iso iso
	rm -rf iso

# Run the ISO in QEMU
# -cdrom: boot from our ISO image
# -m 32M: give the virtual machine 32MB of RAM
# -nographic + serial stdio: show serial output in the terminal (useful later)
run: tinyos.iso
	qemu-system-i386 -cdrom tinyos.iso -m 32M

# Run with GDB debugging enabled
# -s: open a GDB server on localhost:1234
# -S: pause the CPU at startup and wait for GDB to connect
debug: tinyos.iso
	qemu-system-i386 -cdrom tinyos.iso -m 32M -s -S

clean:
	rm -f $(ALL_OBJS) kernel.elf tinyos.iso
	rm -rf iso
EOF */