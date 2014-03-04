#include <stdint.h>
#include "I2C.h"
#include "UART.h"
#include "lcd.h"
#include "SysTick.h"
#include "LPC1114.h"
#include "GPIO.h"

#define PCF8574_I2C_ADDRESS		0x22

volatile int stepper_speed = 0;
volatile int stepper_direction = 1;		// 1: Foreward,   -1: Reverse

void UART_RX_callback(uint8_t data) {

	if (data == 'a') {
		if (stepper_speed < 10) stepper_speed++;
	} else {
		if (stepper_speed > -10) stepper_speed--;
	}
	
	stepper_direction = (stepper_speed >= 0) ? 1 : -1;
}

void Stepper_callback(uint32_t state) {
	static uint32_t stepper_sequence[] = {0x0A, 0x09, 0x05, 0x06};		// This generates a memcpy() if changed to uint8_t[]
	static int stepper_state = 0;
	static uint32_t interval_counter = 0;
	
	uint8_t temp;
	
	// Steppr needs to move ?
	if (stepper_speed != 0) {
		
		// Define a timer interval i.e. invesely proportional to the speed
		interval_counter++;
		
		if (interval_counter >= (21 - 2 * stepper_speed * stepper_direction)) {
			
			// Reset interval counter
			interval_counter = 0;
			
			// Toggle LED
			GPIO1DATA ^= (1 << PIO1_9);
			
			// Issue the next stepper motor instruction based on current state and direction
			if (stepper_direction == -1) {
			
				stepper_state--;
				
				// Reset index
				if (stepper_state == -1) stepper_state = 3;
			
				// Write stepper sequence to I2C
				temp = (uint8_t) stepper_sequence[stepper_state];
				I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
			
			} else {
				
				stepper_state++;
			
				// Reset index
				if (stepper_state == 4) stepper_state = 0;
				
				// Write stepper sequence to I2C
				temp = (uint8_t) stepper_sequence[stepper_state];
				I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
			}	
		}
	
	} else {
	
		// Turn off Stepper coils/windings
		temp = 0xFF;
		I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
	}

}


void main() {
	
	// Initialize LCD (which in turn initializes I2C and SysTick) and Turn ON backlight
	lcd_init();	
	lcd_set_backlight(LCD_BACKLIGHT_ON);
	
	// Initilize UART and GPIO
	UART_init();
	GPIO_init();
		
	// Create and setup Steppr timer
	Timer_t stepper_timer = { 1, 0, 0, 0, Stepper_callback };
	SysTick_run(&stepper_timer);
	
	// Spped control instruction via UART 
	UART_read(UART_RX_callback);
	
	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}
	
}