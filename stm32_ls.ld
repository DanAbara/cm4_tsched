ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rw): ORIGIN = 0x08000000, LENGTH = 512K
	SRAM(rwx): ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
	.text :						/* spacing before colon is important */
	{
		*(.isr_vector)
		*(.text) 				/* get the .text of all input files */
		*(.text.*)				/* merge all .text section */
		*(.init)				/* include the .init section as part of .text */
		*(.fini)				/* include the .fini section as part of .text */
		*(.rodata)		
		*(.rodata.*)			/* merge all .rodata section */
		. = ALIGN(4);			/* Align to the next word/32-bit/4-byte boundary */
		_etext = .;				/* store end of .text section/start of .data section into loc. counter */
	}> FLASH 					/* stored in FLASH, no load address */
	
	_la_data = LOADADDR(.data); /* load address of data section/start of data section in FLASH */

	.data :
	{
		_sdata = .; 			/* start of .data section */
		*(.data)
		*(.data.*)				/* merge all data section */
		_edata = .;				/* end of .data section */
		. = ALIGN(4);			/* align to the next 4-byte boundary */
	}>SRAM AT> FLASH 			/* load address is FLASH (load from flash) virtual address is RAM (copy to RAM) */
		
	.bss :
	{
		_sbss = .;
		__bss_start__ = _sbss; 	/* to fix error in syscalls linking with newlib-nano */
		*(.bss)
		*(.bss.*)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;	/* to fix error in syscalls linking with newlib-nano */
		. = ALIGN(4);			/* Align to next word for Heap, heap starts after bss section */
		end = .;				/* heap start address */
		__end__ = .;			/* for semi hosting lib */
	}> SRAM 					/* stored in RAM */
}