#include <stdint.h>
#include "LPC1114.h"

// Sections as defined in the linker script
extern uint32_t __data_load; 
extern uint32_t __data_start;
extern uint32_t __data_end;
extern uint32_t __bss_start;
extern uint32_t __bss_end;
extern uint32_t __ram_end;

// Application entry point main() declaration
int main(void);

// Default Handler
void Default_Handler(void) {
	while(1);
}

// Interrupt/Exception Handlers
void Reset_Handler(void);
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C_Handler(void) __attribute__ ((weak, alias("Default_Handler")));
void UART_Handler(void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO1_Handler(void) __attribute__ ((weak, alias("Default_Handler")));

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
	I2C_Handler,		// IRQ15
	0,					// IRQ16
	0,					// IRQ17
	0,					// IRQ18
	0,					// IRQ19
	0,					// IRQ20
	UART_Handler,		// IRQ21
	0,					// IRQ22
	0,					// IRQ23
	0,					// IRQ24
	0,					// IRQ25
	0,					// IRQ26
	0,					// IRQ27
	0,					// IRQ28
	0,					// IRQ29
	GPIO1_Handler,		// IRQ30
	0					// IRQ31
};

void SysPLL_init(void) {
	
	// Set Post and Feedback dividers (Input=12Mhz, Output=48Mhz, M=4, MSEL,M-1=3 , P=2)
	SYSPLLCTRL = (0x03 << SYSPLLCTRL_MDIV_BIT) | (SYSPLLCTRL_PDIV_02 << SYSPLLCTRL_PDIV_BIT);
	
	// Power-up PLL block
	PDCONFIG =  (PDCONFIG_SYSPLL_ON << PDCONFIG_SYSPLL_BIT) | (PDCONFIG & ~(1 << PDCONFIG_SYSPLL_BIT));
	
	// Wait for PLL lock 
	while(SYSPLLSTAT != SYSPLLSTAT_LOCKED);
	
	// Select PLL as main clock source
	MAINCLKSEL = MAINCLKSEL_PLLOUT;
	MAINCLKUEN = MAINCLKUEN_OFF;
	MAINCLKUEN = MAINCLKUEN_ON;
	
}

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
	
	// Initialize System PLL to 48Mhz
	SysPLL_init();
	
	// Call main()
	main();
	
	// Infinite loop
	while(1);
}





