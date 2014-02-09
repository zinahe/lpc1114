#include "lcd.h"

int main() {

	// Initialize LCD (which in turn initializes I2C and SysTick)
	lcd_init();
		
    // Turn on LCD back-light
    lcd_set_backlight(LCD_BACKLIGHT_ON);

	while(1) {
		lcd_write("Hello New");
		wait(75);
		lcd_clear();
		
		lcd_write("Hello Old");
		wait(75);
		lcd_clear();
	}
		
}





