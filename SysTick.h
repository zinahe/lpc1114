#include <stdint.h>
#include "LPC1114.h"

// SysTick event counter
volatile uint32_t SysTick_counter;
void SysTick_init(void);
void wait(uint32_t time);

void wait(uint32_t time) {
	
	// Reset counter
	SysTick_counter = 0;
	
	// Wait until SysTick increments counter enough times
	while (SysTick_counter < time);
	
}

void SysTick_init(void) {
	
	// SysTick Reload Value Register, 10ms interval using the Internal RC @12Mhz (pp. 403)
	SYSTICK_RVR = 0x1D4BF;   
	
	// Clear Current Value Register (pp. 403)
	SYSTICK_CVR = 0; 						
	
	// Enable SysTick, on CPU clock and Enable SysTick interrupt (pp. 402)
	SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT) | (1 << SYSTICK_ENABLE_BIT);
}

void SysTick_Handler(void) {
	SysTick_counter++;
}