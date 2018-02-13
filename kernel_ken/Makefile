CC=gcc
C_SRC=
CFLAGS=-Wall -Wextra -g -m32 -nostdlib -fno-builtin -fno-stack-protector -Iinclude -ggdb -DWITH_FRAME_POINTER -ffreestanding -Tasm/link.ld

.phony: all

# BUILD FUNCTION
build: 
	mkdir -p bin
	nasm -felf32 asm/start.s -o asm/start.o
	$(CC) $(CFLAGS) asm/start.o $(wildcard src/*.c) -o bin/kernel.bin
	rm -f asm/start.o

# CLEANUP THE WHOLE SPACE
clean:
	rm -Rf bin

# TEST THE IMG
all: clean build
	qemu-system-x86_64 -kernel bin/kernel.bin 2> /dev/null

# DEBUG USING GDB use kill in gdb to stop qemu after!!!!
# change the initial breakpoint to your need
debug: clean build
	gdb -q -iex 'set architecture x86_64' \
	-iex 'file bin/kernel.bin' \
	-iex 'symbol-file bin/kernel.bin' \
	-iex 'target remote | qemu-system-x86_64 -gdb stdio -S -kernel bin/kernel.bin 2> /dev/null' \
	-iex 'break break_point'
