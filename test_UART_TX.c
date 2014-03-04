#include "UART.h"
#include "SysTick.h"
#include "GPIO.h"
#include "lcd.h"

/*
void UART_TX_callback(void) {
	
	UART_write("Hi Mamo", 7);
	
	// Toggle LED
	GPIO1DATA ^= (1 << PIO1_9);
}

void UART_RX_callback(uint8_t uart_data) {
	
	data[0] = uart_data;
	
	// Toggle LED
	GPIO1DATA ^= (1 << PIO1_9);
}
*/

int main(void) {

	uint8_t data[] = {0, 0};

	// Initialize LCD (which in turn initializes I2C and SysTick) and Turn ON backlight
	lcd_init();	
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	// Initilize UART and GPIO
	UART_init();
	GPIO_init();
	
	while(1) {
	
		// Read data from UART
		data[0] = UART_read(0);
				
		// Write data back to UART
		UART_write(data, 1);
		
		// Send data to LCD
		lcd_write(data);
	}

}

