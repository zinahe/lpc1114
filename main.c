#include <stdint.h>
#include "LPC1114.h"
#include "lcd.h"
#include "GPIO.h"
#include "SysTick.h"

#define PCF8574_I2C_ADDRESS		0x22

void GPIO_callback(uint32_t data) {
	GPIO1DATA ^= (1 << PIO1_9);
}

int main() {

	// Initialize LCD (which in turn initializes I2C and SysTick)
	lcd_init();
	GPIO_init();
	
	// Turn on LCD back-light
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	// Create and setup a timer
	Timer_t timer = {10, 0, 0, GPIO_callback};
	SysTick_run(&timer);	
	
	while(1) {
		lcd_write("Hello Amanu");
		I2C_write(PCF8574_I2C_ADDRESS, 0xFF);				// I2C LED OFF				
		wait(100);
		lcd_clear();
		
		lcd_write("Hello Abel");
		I2C_write(PCF8574_I2C_ADDRESS, 0x00);				// I2C LED ON
		wait(100);
		lcd_clear();
	}
		
}





