#include <stdint.h>
#include "LPC1114.h"

typedef struct Timer {
	uint32_t interval;
	uint32_t counter;
	uint32_t state;
		
	void (*callback)(uint32_t data);
} Timer_t;

volatile uint32_t SysTick_counter;
volatile Timer_t *SysTick_timer;

// Delarations
void SysTick_init(void);
void wait(uint32_t time);
void SysTick_run(Timer_t *);


// Definitions
void SysTick_run(Timer_t *timer) {
	SysTick_timer = timer;
	
	// Enable SysTick block
	SYSTICK_CSR |= (1 << SYSTICK_ENABLE_BIT);
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
}

void SysTick_Handler(void) {

	SysTick_counter++;
	SysTick_timer->counter++;
		
	if (SysTick_timer->interval == SysTick_timer->counter) {
		SysTick_timer->callback(SysTick_timer->state);
		SysTick_timer->counter = 0;
	}
	
}