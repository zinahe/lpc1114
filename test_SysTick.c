#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"
#include "GPIO.h"
#include "I2C.h"
#include "lcd.h"

#define PCF8574_I2C_ADDRESS		0x22

// I2C byte data
uint8_t i2c_data;

void GPIO_callback(uint32_t state) {
	GPIO1DATA ^= (1 << PIO1_9);							// Toggle GPIO LED
}

void I2C_callback(uint32_t state) {
	
	i2c_data ^= 0xFF;
	I2C_write(PCF8574_I2C_ADDRESS, &i2c_data, 1);		// Toggle I2C LED		
}

void LCD_callback(uint32_t state) {
	
	lcd_write("Hello");
}

void main() {

	// Initialize GPIO and SysTick
	GPIO_init();
	
	// Initialize lcd (which will intialize SysTick and I2C)
	lcd_init();
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	//SysTick_init();
	//I2C_init();
		
	// Create and setup GPIO timer
	Timer_t gpio_timer = {20, 0, 0, 0, GPIO_callback};
	SysTick_run(&gpio_timer);

	// Create and setup I2C timer
	Timer_t i2c_timer = {70, 0, 0, 0, I2C_callback};
	SysTick_run(&i2c_timer);	
	
	// Create and setup LCD timer
	Timer_t lcd_timer = {200, 0, 0, 0, LCD_callback};
	SysTick_run(&lcd_timer);
	
	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}

}

