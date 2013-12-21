#include <stdint.h>

// Sections as defined in the linker script
extern uint32_t __data_load; 
extern uint32_t __data_start;
extern uint32_t __data_end;
extern uint32_t __bss_start;
extern uint32_t __bss_end;
extern uint32_t __ram_end;

// Application entry point main() declaration
int main(void);

// Interrupt/Exception handler declarations
void Reset_Handler(void);
void SysTick_Handler(void);
void I2C_Handler(void);

// Interrupt Vector at address 0x00000000
void (* __vectors[]) (void) __attribute__ ((section(".vectors"))) = {
	(void (*) (void)) &__ram_end,
	Reset_Handler,		// RESET 
    0,          		// NMI  
    0,          		// Hard Fault  
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// SVCall  
    0,          		// Reserved 
    0,          		// Reserved 
    0,          		// PendSV  
    SysTick_Handler,    // SysTick  
	0,					// IRQ0
	0,					// IRQ1
	0,					// IRQ2
	0,					// IRQ3
	0,					// IRQ4
	0,					// IRQ5
	0,					// IRQ6
	0,					// IRQ7
	0,					// IRQ8
	0,					// IRQ9
	0,					// IRQ10
	0,					// IRQ11
	0,					// IRQ12
	0,					// IRQ13
	0,					// IRQ14
	I2C_Handler			// IRQ15
};

void Reset_Handler(void) {

	uint32_t *src, *dst;
	
	// Copy .data from Flash to RAM
	src = &__data_load;
	dst = &__data_start;
	while (dst < &__data_end) {
		*dst++ = *src++;
	}
	
	// Clear .BSS section
	dst = &__bss_start;
	while (dst < &__bss_end) {
		*dst++ = 0;
	}
	
	// Call main()
	main();
	
	// Infinite loop
	while(1);
}





