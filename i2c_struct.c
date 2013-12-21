#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"
#include "I2C.h"
#include "GPIO.h"

int main() {

	// Initialize peripherals
	SysTick_init();
	GPIO_init();
	I2C_init();
	
	// Loop
	while(1) {
	
		GPIO1DATA |= (1 << PIO1_9);			// LED ON
		I2C_write(0xFF);
		wait(100);
		
		GPIO1DATA &= ~(1 << PIO1_9);   		// LED OFF
		I2C_write(0x00);
		wait(10);
	} 
	
}






