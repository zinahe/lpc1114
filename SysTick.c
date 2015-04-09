#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"

static volatile uint32_t Wait_counter;
static Timer_t * Timer_first;

void (* volatile Timer_callbacks[])(uint32_t) = { 0, 0, 0, 0, 0 };
volatile uint32_t Callback_counter;

void SysTick_init(void) {
	
	// SysTick Reload Value Register, 10ms interval using the Internal RC @12Mhz (pp. 403)
	//SYSTICK_RVR = 0x1D4BF;  
	
	// SysTick Reload Value Register, 1ms interval using the Internal RC @48Mhz (pp. 403)
	//SYSTICK_RVR = 0xBB7F; 
	
	// SysTick Reload Value Register, 250usec interval using the Internal RC @48Mhz (pp. 403)
	SYSTICK_RVR = 0x2EDF; 	
		
	// Clear Current Value Register (pp. 403)
	SYSTICK_CVR = 0; 						
	
	// Enable SysTick counter, Select clock source (1=CPU, 0=CPU/2), Enable SysTick interrupt (pp. 402)
	//SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT) | (1 << SYSTICK_ENABLE_BIT);
	
	// Select clock source (1=CPU, 0=CPU/2), Enable SysTick interrupt (pp. 402)
	SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT);
	
	// Initialize timer pointers
	Timer_first = 0;
	
	// Initialize callback counter
	Callback_counter = 0;
}

void SysTick_add(Timer_t * timer) {
	Timer_t * _timer;
	int _exists = 0;

	if (Timer_first == 0) {
		
		// Attach the first timer to pointers
		Timer_first = timer;
		//Timer_current = timer;
		timer->next = timer;
		
		// Enable SysTick counter on first timer
		SYSTICK_CSR |= (1 << SYSTICK_ENABLE_BIT);
	} else {

		// Search if it already exists in queue	
		_timer = Timer_first;
		do {

			_exists = (_timer == timer);
			if (_exists) break;

			if (_timer->next == Timer_first) {
				_timer->next = timer;
				timer->next = Timer_first;
				break; 		// NOTE: This will prevent testing of new timer
			}

			_timer = _timer->next;	
		} while (_timer != Timer_first);

/*		if (!_exists){
			// Add new timer between the first and current timer
			timer->next = Timer_first;
			Timer_current->next = timer;
			Timer_current = timer;
		} */
	}
	
}

void SysTick_remove(Timer_t * timer) {

	Timer_t * _timer;

	if (Timer_first != 0) {

		_timer = Timer_first;
		do {
			// Check if the next timer is the one to be removed	
			if (_timer->next == timer) {

				_timer->next = timer->next;
				timer->next = 0;
				
				if (Timer_first == timer)
					Timer_first = _timer->next;

				break;
			}

			_timer = _timer->next;	
		} while (_timer != Timer_first);

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

	Timer_t * _timer;
	Wait_counter++;
	
	if (Timer_first != 0) {

		_timer = Timer_first;
		do {
			// Increment timer counter
			_timer->counter++;
		
			// TODO: Maintain timer state
		
			// Check current count against timer interval setting
			if (_timer->interval == _timer->counter) {
				
				// Enqueue callback function
				Timer_callbacks[Callback_counter++] = _timer->callback;

				// Reset counter	
				_timer->counter = 0;
			}
			
			_timer = _timer->next;
		} while (_timer != Timer_first);
	}
	
}

//static void (* volatile Timer_callbacks[])(uint32_t) = { 0, 0, 0, 0, 0 };
//static volatile uint32_t Callback_counter;

/*	
	if (Timer_first != 0) {

		do {
			// Check if the next timer is the one to be removed	
			if (Timer_current->next == timer) {
				Timer_current->next = timer->next;
				timer->next = 0;
			}

			// Jump to the next timer
			Timer_current = Timer_current->next;	

			// Check if it's the last timer to be removed
			if (Timer_current->next == 0) {
				Timer_first = 0;
			}

		} while (Timer_current->next != Timer_first);

	} */
