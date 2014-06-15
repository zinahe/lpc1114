#include "LPC1114.h"
#include "UART.h"
#include "GPIO.h"

int main(void) {

	uint8_t data[] = {0, 0};
	
	// Initilize UART and GPIO
	GPIO_init();
	UART_init();
		
	while(1) {
	
		// Read data from UART
		data[0] = UART_read(0);
		
		// Toggle LED if 'a'
		if (data[0] == 'a')
			GPIO1DATA ^= (1 << PIO1_9);
				
		// Write data back to UART
		UART_write(data, 1);
		//UART_write("Hi Mamo", 7);
		
	}

}

