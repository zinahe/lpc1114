#include "UART.h"
#include "SysTick.h"
#include "GPIO.h"

void send_message(uint8_t *);

int main(void) {

	// Initialize SysTick and UART blocks
	SysTick_init();
	UART_init();
	GPIO_init();
	
	// Send 'h' forever
	while(1) {
		send_message("Hi Lily");
		wait(100);
		GPIO1DATA ^= (1 << PIO1_9);
	}

}

void send_message(uint8_t *message) {
	while(*message != 0) {
		UART_send(*message);
		wait(1);
		message++;
	}
}