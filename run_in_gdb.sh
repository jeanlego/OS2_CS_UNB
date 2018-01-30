#!/bin/sh
add-symbol-file stage0.elf 0x7c00
add-symbol-file stage1.elf 0x7e00
target remote | qemu-system-x86_64 -S -gdb stdio -m 16 -boot c -fda floppy.img
