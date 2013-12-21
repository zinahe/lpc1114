OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

/* Could be used as part of --gc-sections Linker option */
ENTRY(Reset_Handler)

/* Memory configuration for LPC1114 */

MEMORY {
    flash   :   ORIGIN = 0x00000000, LENGTH = 32k
    ram     :   ORIGIN = 0x10000000, LENGTH = 8k
}

SECTIONS {
    . = ORIGIN(flash);

    .vectors : {
        KEEP(*(.vectors))
    } >flash

    .text : {
        *(.text .text*)
		*(.rodata .rodata*)
        /* __data_load = .;  */
    } >flash

    .data : {
		__data_load = LOADADDR(.data); 
        __data_start = .;
        *(.data .data*)
        __data_end = .;
    } >ram AT >flash

    .bss : {
        __bss_start = .;
        *(.bss .bss*)
        __bss_end = .;
    } >ram

    . = ALIGN(4);
	
	/* __ram_end = 0x10001FFC; */
	/* __ram_end = 0x10002000; */
	__ram_end = ORIGIN(ram) + LENGTH(ram); 
}
 
_end = .;
PROVIDE(end = .);
