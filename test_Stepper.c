#include <stdint.h>
#include "I2C.h"
#include "UART.h"
#include "lcd.h"

#define PCF8574_I2C_ADDRESS		0x22

void main() {

	// Stepper sequence
	uint32_t index = 0;
	uint32_t sequence[] = {0x0A, 0x09, 0x05, 0x06};		// This generates a memcpy() if changed to uint8_t[]
	uint8_t temp;
		
	// UART data
	uint8_t data[] = {0, 0};
	
	// Initialize LCD (which in turn initializes I2C and SysTick) and Turn ON backlight
	lcd_init();	
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	// Initialize UART
	UART_init();
		
	// Turn off Stepper coils/windings
	temp = 0xFF;
	I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
	
	// Read I2C switch input and write it back to I2C led output
	while(1) {
		
		// Wait for a character from UART (PC)
		data[0] = UART_read();
		
		// Write chacter to LCD (write until null/0)
		//lcd_write(data);
	
		
		if (data[0] == 'a') {
		
			// Write stepper sequence to I2C
			temp = (uint8_t) sequence[index--];
			I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
		
			// Reset index
			if (index == -1) index = 3;
		
		} else {
			// Write stepper sequence to I2C
			temp = (uint8_t) sequence[index++];
			I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
		
			// Reset index
			if (index == 4) index = 0;
		}
		

	}

}