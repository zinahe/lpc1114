#include "LPC1114.h"
#include "SysTick.h"
#include "GPIO.h"

volatile uint32_t flag;

void GPIO_callback(uint32_t);
void main_callback(uint32_t);

Timer_t gpio_timer1 = {2000, 0, 0, 0, GPIO_callback};
Timer_t gpio_timer2 = {1000, 0, 0, 0, GPIO_callback};
Timer_t main_timer = {16000, 0, 0, 0, main_callback};

void GPIO_callback(uint32_t state) {

	// Toggle GPIO LED
	GPIO1DATA ^= (1 << PIO1_9);							
}

void main_callback(uint32_t state) {

	if (flag == 1) {
		// Stop timer 1 and start timer 2	
		SysTick_remove(&gpio_timer1);
		SysTick_add(&gpio_timer2);
		flag = 2;
	} else {
		// Stop timer 2 and start timer 1	
		SysTick_remove(&gpio_timer2);
		SysTick_add(&gpio_timer1);
		flag = 1;
	}
}

void main() {

	// Initialize SysTick, GPIO and UART
	GPIO_init();
	SysTick_init();
	
	flag = 1;

	SysTick_add(&gpio_timer1);
	SysTick_add(&main_timer);
	
	// Despatch timer callbacks if available; or just wait
	SysTick_run();
	
}

