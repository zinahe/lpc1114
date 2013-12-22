#include <stdint.h>
#include "LPC1114.h"
#include "lcd.h"
#include "GPIO.h"

#define PCF8574_I2C_ADDRESS		0x22

int main() {

	// Initialize LCD (which in turn initializes I2C and SysTick)
	lcd_init();
	GPIO_init();
	
	// Turn on LCD back-light
	lcd_set_backlight(LCD_BACKLIGHT_ON);

	while(1) {
		lcd_write("Hello Amanu");
		I2C_write(PCF8574_I2C_ADDRESS, 0xFF);				// I2C LED OFF				
		GPIO1DATA |= (1 << PIO1_9);							// GPIO LED ON
		wait(100);
		lcd_clear();
		
		lcd_write("Hello Abel");
		I2C_write(PCF8574_I2C_ADDRESS, 0x00);				// I2C LED ON
		GPIO1DATA &= ~(1 << PIO1_9);						// LED OFF
		wait(100);
		lcd_clear();
	}
		
}





