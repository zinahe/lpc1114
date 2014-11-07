#include "lcd.h"
#include "SysTick.h"

int main() {

	// Initialize LCD (which in turn initializes I2C and SysTick)
	lcd_init();
		
    // Turn on LCD back-light
	// lcd_set_backlight(LCD_BACKLIGHT_ON);
    lcd_backlight_ON();

	while(1) {
		lcd_write("Time to Sleep.");
		wait(4000);
		lcd_clear();
		
		lcd_write("Zinahe Asnake.");
		wait(4000);
		lcd_clear();
	}
		
}





