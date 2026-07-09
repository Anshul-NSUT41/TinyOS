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

CFLAGS = -m32 -ffreestanding -fno-builtin -fno-stack-protector \
         -nostdlib -Wall -Wextra -Iinclude

# ── Linker flags ──────────────────────────────────────────────────────────
LDFLAGS = -m elf_i386 -T linker.ld --oformat=elf32-i386

# ── Source files → object files ───────────────────────────────────────────
C_SRCS   = kernel/kernel.c kernel/vga.c kernel/idt.c kernel/isr.c kernel/irq.c kernel/timer.c kernel/keyboard.c
ASM_SRCS = boot/boot.asm boot/idt_asm.asm 

C_OBJS   = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.asm=.o)
ALL_OBJS = $(ASM_OBJS) $(C_OBJS)

# ── Build targets ─────────────────────────────────────────────────────────
.PHONY: all run debug clean

all: tinyos.iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/boot.o: boot/boot.asm
	$(AS) -f elf32 $< -o $@

kernel.elf: $(ALL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

tinyos.iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg   iso/boot/grub/grub.cfg
	$(GRUB) -o tinyos.iso iso
	rm -rf iso

run: tinyos.iso
	qemu-system-i386 -cdrom tinyos.iso -m 32M

debug: tinyos.iso
	qemu-system-i386 -cdrom tinyos.iso -m 32M -s -S

clean:
	rm -f $(ALL_OBJS) kernel.elf tinyos.iso
	rm -rf iso
EOF