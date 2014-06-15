#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"
#include "GPIO.h"

void GPIO_callback(uint32_t state) {
	GPIO1DATA ^= (1 << PIO1_9);							// Toggle GPIO LED
}

void main() {

	// Initialize SysTick, GPIO and UART
	GPIO_init();
	SysTick_init();
	
	// Create and setup GPIO timer
	Timer_t gpio_timer = {2000, 0, 0, 0, GPIO_callback};
	SysTick_run(&gpio_timer);

	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}

}

