#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"
#include "I2C.h"
#include "GPIO.h"

void GPIO_callback(uint32_t data) {
	GPIO1DATA ^= (1 << PIO1_9);
}

int main() {

	// Initialize peripherals
	SysTick_init();
	GPIO_init();
	I2C_init();
	
	Timer_t timer = {10, 0, 0, GPIO_callback};
	SysTick_run(&timer);
	
	// Loop
	//while(1);
	
	
	
	while(1) {
	
		I2C_write(0xFF);
		wait(100);
		
		I2C_write(0x00);
		wait(10);
	} 
	
}






