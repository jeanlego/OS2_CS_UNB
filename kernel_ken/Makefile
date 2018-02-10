# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

INITIAL_BREAK=__init
DEBUG=release

SRC=\
$(patsubst %.s, %.o, $(wildcard src/include/*.s)) \
$(patsubst %.c, %.o, $(wildcard src/include/*.c)) \
$(patsubst %.c, %.o, $(wildcard src/*.c))

CFLAGS=-Wall -g -m32 -march=i386 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I src -I src/include
ifeq ($(DEBUG),release)
	CFLAGS+=-fomit-frame-pointer
else
	CFLAGS+=-ggdb -DWITH_FRAME_POINTER
endif

LDFLAGS=-melf_i386 -Tsrc/include/link.ld
ASFLAGS=-felf32

# BUILD FUNCTION
all: $(SRC) 
	mkdir -p bin
	ld $(LDFLAGS) -o bin/kernel.bin $(SRC) 
	

# CLEANUP THE WHOLE SPACE
clean:
	rm -Rf bin
	rm -f src/*.o
	rm -f src/include/*.o
	
.s.o:
	nasm $(ASFLAGS) $<

# TEST THE IMG
test:
	qemu-system-i386 -kernel bin/kernel.bin 2> /dev/null

# DEBUG USING GDB use kill in gdb to stop qemu after!!!!
# change the initial breakpoint to your need
debug_test:
	gdb -q -iex 'set architecture i386' \
	-iex 'file bin/kernel.bin' \
	-iex 'symbol-file bin/kernel.bin' \
	-iex 'target remote | qemu-system-i386 -gdb stdio -S -kernel bin/kernel.bin 2> /dev/null' \
	-iex 'break __init'


# RUN THE WHOLE SUITE
full: clean all test	




