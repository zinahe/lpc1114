#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"

static volatile uint32_t Wait_counter;

static Timer_t * Timer_first;
static volatile Timer_t * volatile Timer_current;

static void (* volatile Timer_callbacks[])(uint32_t) = { 0, 0, 0, 0, 0 };
static volatile uint32_t Callback_counter;

// Definitions

void SysTick_init(void) {
	
	// SysTick Reload Value Register, 10ms interval using the Internal RC @12Mhz (pp. 403)
	//SYSTICK_RVR = 0x1D4BF;  
	
	// SysTick Reload Value Register, 1ms interval using the Internal RC @48Mhz (pp. 403)
	//SYSTICK_RVR = 0xBB7F; 
	
	// SysTick Reload Value Register, 250usec interval using the Internal RC @48Mhz (pp. 403)
	SYSTICK_RVR = 0x2EDF; 	
		
	// Clear Current Value Register (pp. 403)
	SYSTICK_CVR = 0; 						
	
	// Enable SysTick, on CPU clock and Enable SysTick interrupt (pp. 402)
	//SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT) | (1 << SYSTICK_ENABLE_BIT);
	SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT);
	
	// Initialize timer pointers
	Timer_first = 0;
	Timer_current = 0;
	
	// Initialize callback counter
	Callback_counter = 0;
}

void SysTick_add(Timer_t * timer) {

	if (Timer_first == 0) {
		
		// Attach the very first timer
		Timer_first = timer;
		Timer_current = timer;
		timer->next = timer;
		
		// Enable SysTick block
		SYSTICK_CSR |= (1 << SYSTICK_ENABLE_BIT);
	} else {
	
		// Add new timer between the first and current timer
		timer->next = Timer_first;
		Timer_current->next = timer;
		Timer_current = timer;
	}
	
}

void SysTick_run(void) {

	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}

}

void wait(uint32_t time) {
	
	// Reset counter
	Wait_counter = 0;
	
	// Enable SysTick block
	SYSTICK_CSR |= (1 << SYSTICK_ENABLE_BIT);
	
	// Wait until SysTick increments counter enough times
	while (Wait_counter < time);
	
}

void SysTick_Handler(void) {

	Wait_counter++;
	
	if (Timer_first != 0) {
		
		do {
			// Icrement timer counter
			Timer_current->next->counter++;
		
			// Toggle timer state
			//Timer_current->next->state ^= 0xFF;
		
			// Reset counter and enqueue callback function pointer
			if (Timer_current->next->interval == Timer_current->next->counter) {
			
				// Timer_current->next->callback(Timer_current->next->state);
				// Timer_current->next->callback(0);
				Timer_callbacks[Callback_counter++] = Timer_current->next->callback;
				
				Timer_current->next->counter = 0;
			}
			
			Timer_current = Timer_current->next;
		} while (Timer_current->next != Timer_first);
	}
	
}
