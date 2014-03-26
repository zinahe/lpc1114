#include "LPC1114.h"
#include "GPIO.h"

#define GPIO1_5		5

void main() {

	// Initialize GPIO Block
	GPIO_init();
	
	// GPIO1_ISENSE defaults to Edge Sensitive trigger

	// GPIO1_IEDGES defaults to GPIO1_IEVENT control
	
	// Enable Rising Edge trigger
	GPIO1_IEVENT = (1 << GPIO1_5);
	
	// Enable GPIO Interrupt
	GPIO1_IENABLE = (1 << GPIO1_5);
	
	// Enable GPIO1 Interrupt in NVIC 
	NVIC_SETENA = (1 << NVIC_GPIO1_BIT);
	
	// Loop
	while(1);
}

void GPIO1_Handler(void) {
	//GPIO1_IMASKSTAT
	
	// Clear GPIO Edge Intrrupt signal
	GPIO1_ICLR = (1 << GPIO1_5);
	
	// Toggle LED
	GPIO1DATA ^= (1 << PIO1_9);
}

/* 
GPIO1_ISENSE
GPIO1_IEDGES
GPIO1_IEVENT
GPIO1_IENABLE
GPIO1_IRAWSTAT
GPIO1_IMASKSTAT
GPIO1_ICLR

void GPIO0_Handler(void) {
	
	
}

void GPIO2_Handler(void) {
	
	
}

void GPIO3_Handler(void) {
	
	
}

*/