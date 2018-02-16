/* vim: tabstop=4 shiftwidth=4 noexpandtab
 * Kernel linker script for x86
 */
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
phys = 0x100000;
SECTIONS
{
	/*
	 * Actual code
	 */
	.text phys : AT(phys) {
		code = .;
		*(.text)
		*(.rodata)
		. = ALIGN(4K);
	}
	/*
	 * Kernel data
	 */
	.data : AT(phys + (data - code))
	{
		data = .;
		*(.data)
		. = ALIGN(4K);
	}
	/*
	 * Statically defined, uninitialized values
	 */
	.bss : AT(phys + (bss - code))
	{
		bss = .;
		*(.bss)
		. = ALIGN(4K);
	}
	/*
	 * End of kernel.
	 */
	end = .;
	/*
	 * Get rid of unnecessary GCC bits.
	 */
	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}
}
