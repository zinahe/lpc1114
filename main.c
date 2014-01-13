#include <stdint.h>
#include "LPC1114.h"
#include "lcd.h"
#include "GPIO.h"
#include "SysTick.h"
#include "I2C.h"

#define PCF8574_I2C_ADDRESS		0x22

void GPIO_callback_ON(uint32_t data) {
	GPIO1DATA |= (1 << PIO1_9);
}

void GPIO_callback_OFF(uint32_t data) {
	GPIO1DATA &= ~(1 << PIO1_9);
}


int main() {

	// Initialize LCD (which in turn initializes I2C and SysTick)
	lcd_init();
	GPIO_init();
		
    // Turn on LCD back-light
    lcd_set_backlight(LCD_BACKLIGHT_ON);
		
	// Create and setup GPIO timer
	Timer_t timer = {35, 0, 0, 0, GPIO_callback_ON};
	SysTick_run(&timer);

	// Create and setup I2C timer
	Timer_t timer2 = {50, 0, 0, 0, GPIO_callback_OFF};
	SysTick_run(&timer2);	
	
	while(1) {
		lcd_write("Hello USA");
		I2C_write(PCF8574_I2C_ADDRESS, 0xFF);				// I2C LED OFF				
		wait(75);
		lcd_clear();
		
		lcd_write("Hello Lily");
		I2C_write(PCF8574_I2C_ADDRESS, 0x00);				// I2C LED ON
		wait(75);
		lcd_clear();
	}
		
}





