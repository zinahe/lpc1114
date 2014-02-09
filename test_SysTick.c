#include <stdint.h>
#include "SysTick.h"
#include "GPIO.h"

void GPIO_callback_ON(uint32_t data) {
	GPIO1DATA |= (1 << PIO1_9);
}

void GPIO_callback_OFF(uint32_t data) {
	GPIO1DATA &= ~(1 << PIO1_9);
}

void main() {

	// Initialize GPIO and SysTick
	GPIO_init();
	SysTick_init();
		
	// Create and setup GPIO timer
	Timer_t timer = {100, 0, 0, 0, GPIO_callback_ON};
	SysTick_run(&timer);

	// Create and setup I2C timer
	Timer_t timer2 = {70, 0, 0, 0, GPIO_callback_OFF};
	SysTick_run(&timer2);	

	// Wait
	while(1);

}