#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"
#include "GPIO.h"
#include "SYSPLL.h"

void GPIO_callback(uint32_t state) {
	// Toggle GPIO LED
	GPIO1DATA ^= (1 << PIO1_9);							
}

void main() {

	// Initialize GPIO and SysTick
	GPIO_init();
	SysTick_init();
	
	SYSPLL_init();
	
	// Create and setup GPIO timer
	Timer_t gpio_timer = {500, 0, 0, 0, GPIO_callback};
	SysTick_run(&gpio_timer);
	
	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}
}


