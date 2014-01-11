#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"

volatile uint32_t SysTick_counter;
volatile Timer_t * volatile SysTick_timer_start;
volatile Timer_t * volatile SysTick_timer_current;

// Definitions

//void SysTick_run(volatile Timer_t * volatile timer) {
void SysTick_run(Timer_t * timer) {

	if (SysTick_timer_start == 0) {
		
		// Attach the very first timer
		SysTick_timer_start = timer;
		SysTick_timer_current = timer;
		timer->next = timer;
		
		// Enable SysTick block
		SYSTICK_CSR |= (1 << SYSTICK_ENABLE_BIT);
	} else {
	
		// Add new timer between start and current timers
		timer->next = SysTick_timer_start;
		SysTick_timer_current->next = timer;
		SysTick_timer_current = timer;
	}
	
}

void wait(uint32_t time) {
	
	// Reset counter
	SysTick_counter = 0;
	
	// Enable SysTick block
	SYSTICK_CSR |= (1 << SYSTICK_ENABLE_BIT);
	
	// Wait until SysTick increments counter enough times
	while (SysTick_counter < time);
	
}

void SysTick_init(void) {
	
	// SysTick Reload Value Register, 10ms interval using the Internal RC @12Mhz (pp. 403)
	SYSTICK_RVR = 0x1D4BF;   
	
	// Clear Current Value Register (pp. 403)
	SYSTICK_CVR = 0; 						
	
	// Enable SysTick, on CPU clock and Enable SysTick interrupt (pp. 402)
	//SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT) | (1 << SYSTICK_ENABLE_BIT);
	SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT);
	
	// Initialize timer pointers
	SysTick_timer_start = 0;
	SysTick_timer_current = 0;
}

void SysTick_Handler(void) {

	SysTick_counter++;
	
	if (SysTick_timer_start != 0) {
		
		do {
			// Icrement timer counter
			SysTick_timer_current->next->counter++;
		
			// Toggle timer state
			//SysTick_timer_current->next->state ^= 0xFF;
		
			// Reset counter and fire callback function
			if (SysTick_timer_current->next->interval == SysTick_timer_current->next->counter) {
				SysTick_timer_current->next->callback(SysTick_timer_current->next->state);
				SysTick_timer_current->next->counter = 0;
			}
			
			SysTick_timer_current = SysTick_timer_current->next;
		} while (SysTick_timer_current->next != SysTick_timer_start);
	}
	
}
