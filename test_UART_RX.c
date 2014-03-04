#include "UART.h"
#include "SysTick.h"
#include "GPIO.h"
#include "LPC1114.h"

volatile uint8_t uart_data[] = {0, 0};

void UART_RX_callback(uint8_t rx_data) {
	
	//uart_data[0] = rx_data;
	
	if (rx_data == 'a') {
		// Toggle LED
		GPIO1DATA ^= (1 << PIO1_9);
	}
}

int main(void) {

	// Initilize UART and GPIO
	UART_init();
	GPIO_init();
	
	// Turn ON LED
	GPIO1DATA = (1 << PIO1_9);
	
	// UART Read with a callback
	UART_read(UART_RX_callback);
	
	while(1);
}


