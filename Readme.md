# TinyOS

<p align="center">
  <img src="assets/boot-screen.png" width="800">
</p>

<p align="center">
  A simple 32-bit x86 operating system kernel built from scratch.
</p>

Currently, TinyOS can:

- Boot using GRUB (Multiboot)
- Enter protected mode through GRUB
- Load a custom kernel
- Write directly to VGA text-mode memory
- Display text on the screen
- Build into a bootable ISO image
- Run inside QEMU

---

## Demo

```text
--------------------------------
Welcome to TinyOS v0.1
--------------------------------

Kernel loaded successfully.
VGA text mode: OK
```

---

## Project Structure

```text
tinyOS/
├── assets/
│   └── Boot-screen.png
├── boot/
│   └── boot.asm        # Kernel entry point and multiboot header
│
├── kernel/
│   ├── kernel.c        # Main kernel code
│   ├── vga.c           # VGA text-mode driver
│   └── vga.h
│
├── include/            # Future headers
│
├── grub.cfg            # GRUB configuration
├── linker.ld           # Kernel memory layout
├── Makefile            # Build system
└── README.md
```

---

## Build Requirements

- GCC (32-bit support)
- NASM
- GNU LD
- GRUB tools
- QEMU

Ubuntu:

```bash
sudo apt update

sudo apt install \
build-essential \
nasm \
grub-pc-bin \
xorriso \
qemu-system-x86
```

---

## Build

```bash
make
```

This generates:

```text
kernel.elf
tinyos.iso
```

---

## Run

```bash
make run
```

---

## Clean

```bash
make clean
```

---

## Current Features

- [x] GRUB Multiboot support
- [x] Custom linker script
- [x] VGA text output
- [x] Bootable ISO generation
- [x] QEMU support

---

## Planned Features

- [ ] Keyboard driver
- [ ] Interrupt handling (IDT)
- [ ] Memory management
- [ ] Heap allocator
- [ ] Shell
- [ ] Filesystem
- [ ] Multitasking

---

## Learning Goals

This project exists primarily to explore:

- Computer Architecture
- Operating Systems
- x86 Assembly
- Memory Layout
- Linking and Loading
- Low-level C Programming

---

## License

MIT License
