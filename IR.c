#include "LPC1114.h"
#include "GPIO.h"
#include "SysTick.h"
#include "UART.h"

#define GPIO1_5		5

volatile uint32_t IR_counter;

void IR_Timer_callback(uint32_t state) {
	
	uint16_t data;
	
	// Toggle LED
	GPIO1DATA ^= (1 << PIO1_9);
		
	data = (uint8_t) IR_counter;
	
	UART_write(&data, 2);
	
	IR_counter = 0;
}

void main() {

	// Initialize SysTick, GPIO and UART
	GPIO_init();
	SysTick_init();
	UART_init();
	
	// ------------- GPIO Interrupt Configuration ------------------
	// GPIO1_ISENSE defaults to Edge Sensitive trigger

	// GPIO1_IEDGES defaults to GPIO1_IEVENT control
	// Configure both edges trigger
	GPIO1_IEDGES = (1 << GPIO1_5);
		
	// Enable Rising Edge trigger
	//GPIO1_IEVENT = (1 << GPIO1_5);
	
	// Enable GPIO Interrupt
	GPIO1_IENABLE = (1 << GPIO1_5);
	
	// Enable GPIO1 Interrupt in NVIC 
	NVIC_SETENA = (1 << NVIC_GPIO1_BIT);
	// ------------- GPIO Interrupt Configuration ------------------
	
	
	IR_counter = 0;
	
	// Create and setup IR timer
	Timer_t IR_timer = { 1000, 0, 0, 0, IR_Timer_callback };
	SysTick_run(&IR_timer);	
	
	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}
	
}

void GPIO1_Handler(void) {

	// GPIO1_IMASKSTAT can be used to identify which pin is causing the interrupt 
	
	// Clear GPIO Edge Intrrupt signal
	GPIO1_ICLR = (1 << GPIO1_5);
	
	// Toggle LED
	GPIO1DATA ^= (1 << PIO1_9);
	
	if (GPIO1DATA & (1 << GPIO1_5)) {
		
		// Falling edge
		IR_counter++;
	} else {
	
		// Rising edge
	
	}
}

