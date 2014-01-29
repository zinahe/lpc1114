#include "UART.h"
#include "SysTick.h"
#include "GPIO.h"
#include "lcd.h"

//void send_message(uint8_t *);
void UART_callback(void) {
	
	UART_write("Hi Mamo", 7);
	
	// Toggle LED
	GPIO1DATA ^= (1 << PIO1_9);
}

int main(void) {

	uint8_t data[] = {0, 0};

	// Initialize LCD (which in turn initializes I2C and SysTick) and Turn ON backlight
	lcd_init();	
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	// Initilize UART and GPIO
	UART_init();
	GPIO_init();
	
	// Create and setup timer for UART TX 
	Timer_t timer = {50, 0, 0, 0, UART_callback};
	SysTick_run(&timer);
		
	while(1) {
	
		// Read data from UART
		data[0] = UART_read();
		
		// Send data to LCD
		lcd_write(data);
		
	}

}

