#include "LPC1114.h"

#include "I2C.h"
#include "UART.h"
#include "SysTick.h"
#include "GPIO.h"

#define PCF8574_I2C_ADDRESS		0x22
#define HALF_STEP				1
#define FULL_STEP 				2

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
	// These array definitions will generates a memcpy() if changed to uint8_t[]
	static uint32_t stepper_sequence[] = {0x0A, 0x0B, 0x09, 0x0D, 0x05, 0x07, 0x06, 0x0E};		// L298 sequence
	//static uint32_t stepper_sequence[] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};		// Polulu 4988 sequence
	
	static int stepper_state = 0;
	static uint32_t interval_counter = 0;
	static uint32_t stepper_mode = HALF_STEP;
	
	uint8_t temp;
	
	// Steppr needs to move ?
	if (stepper_speed != 0) {
		
		// Define a timer interval i.e. invesely proportional to the speed
		interval_counter++;
		
		// Count up to current speed
		if (interval_counter >= (21 - 2 * stepper_speed * stepper_direction)) {
			
			// Reset interval counter
			interval_counter = 0;
			
			// Toggle LED
			GPIO1DATA ^= (1 << PIO1_9);
			
			// Issue the next stepper motor instruction based on current state and direction
			if (stepper_direction == -1) {
			
				//stepper_state--;
				stepper_state -= stepper_mode;
				
				// Reset index
				//if (stepper_state == -1) stepper_state = 3;
				if (stepper_state < 0) stepper_state += 8;
			
				// Write stepper sequence to I2C
				temp = (uint8_t) stepper_sequence[stepper_state];
				
				// Use for Polulu 4988
				//temp |= 0x04;
				
				I2C_write(PCF8574_I2C_ADDRESS, &temp, 1);
			
			} else {
				
				//stepper_state++;
				stepper_state += stepper_mode;
			
				// Reset index
				if (stepper_state == 8) stepper_state = 0;
				
				// Write stepper sequence to I2C
				temp = (uint8_t) stepper_sequence[stepper_state];
				
				// Use for Polulu 4988
				//temp &= 0xFB;
				
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
	
	// Initialize SysTick, GPIO, I2C and UART
	GPIO_init();
	SysTick_init();
	I2C_init();
	UART_init();
		
	// Create and setup Steppr timer
	Timer_t stepper_timer = { 20, 0, 0, 0, Stepper_callback };
	SysTick_run(&stepper_timer);
	
	// Spped control instruction via UART 
	UART_read(UART_RX_callback);
	
	// Despatch timer callbacks if available; or idle
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}
	
}