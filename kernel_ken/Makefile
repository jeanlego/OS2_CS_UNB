CC=gcc
AS=nasm
C_SRC=$(wildcard src/*.c)
ASM_SRC=$(wildcard asm/*.s)
OBJ_SRC=$(patsubst %.s, %.o, $(ASM_SRC))
LINK_DEF=link.ld
KERNEL_OUT=bin/kernel.bin
ARCH=x86_64
ASMFLAGS=-felf32

CFLAGS=-Wall -Wextra \
-g -ggdb \
-m32 -nostdlib -fno-builtin -fno-stack-protector -ffreestanding \
-Iinclude -DWITH_FRAME_POINTER  -T$(LINK_DEF)

.phony: all

# BUILD FUNCTION
build: $(OBJ_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJ_SRC) $(C_SRC) -o $(KERNEL_OUT)
	rm -f asm/*.o

# CLEANUP THE WHOLE SPACE
clean:
	rm -Rf bin
	
.s.o:
	$(AS) $(ASMFLAGS) $<

# TEST THE IMG
all: clean build
	qemu-system-$(ARCH) -kernel $(KERNEL_OUT) 2> /dev/null

# DEBUG USING GDB use kill in gdb to stop qemu after!!!!
# change the initial breakpoint to your need
debug: clean build
	gdb -q -iex 'set architecture $(ARCH)' \
	-iex 'file $(KERNEL_OUT)' \
	-iex 'symbol-file $(KERNEL_OUT)' \
	-iex 'target remote | qemu-system-$(ARCH) -gdb stdio -S -kernel $(KERNEL_OUT) 2> /dev/null' \
	-iex 'break break_point'
