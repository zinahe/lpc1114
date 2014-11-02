#include "LPC1114.h"
#include "SysTick.h"
#include "GPIO.h"

void GPIO_callback(uint32_t state) {

	// Toggle GPIO LED
	GPIO1DATA ^= (1 << PIO1_9);							
}

void main() {

	// Initialize SysTick, GPIO and UART
	GPIO_init();
	SysTick_init();
	
	// Create and add a timer callback for GPIO 
	Timer_t gpio_timer = {4000, 0, 0, 0, GPIO_callback};
	SysTick_add(&gpio_timer);

	// Despatch timer callbacks if available; or just wait
	SysTick_run();
	
}

