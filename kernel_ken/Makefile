# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

SOURCES=\
src/boot.o \
src/main.o \
src/monitor.o \
src/common.o \
src/descriptor_tables.o \
src/isr.o \
src/interrupt.o \
src/gdt.o \
src/timer.o \
src/kheap.o \
src/paging.o
		

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-melf_i386 -Tsrc/link.ld
ASFLAGS=-felf

# BUILD FUNCTION
all: $(SOURCES) 
	mkdir -p bin
	ld $(LDFLAGS) -o bin/kernel $(SOURCES)
	
# RUN THE WHOLE SUITE
full: 
	clean all img test

# CLEANUP THE WHOLE SPACE
clean:
	rm -Rf bin
	rm src/*.o
	
.s.o:
	nasm $(ASFLAGS) $<

# WRITE THE IMG FILE	
img: 
	/sbin/losetup /dev/loop0 floppy.img
	mkdir -p floppy_dir
	mount /dev/loop0 floppy_dir
	cp bin/kernel floppy_dir/kernel
	umount /dev/loop0
	rm -Rf floppy_dir
	/sbin/losetup -d /dev/loop0

# TEST THE IMG
test:
	qemu-system-i386 -fda floppy.img

# DEBUG USING GDB
debug_test:
	qemu-system-i386 -fda floppy.img

	




