#include <stdint.h>
#include "LPC1114.h"
#include "lcd.h"

int main() {

	char * message = "Hello Amanuel";

	// Initialize LCD (which in turn initializes I2C and SysTick)
	lcd_init();
	
	// Turn on LCD back-light
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	// Display message
	lcd_write(message);
	
	while(1);
	
}





