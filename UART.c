#include <stdint.h>
#include "LPC1114.h"
#include "UART.h"

void UART_init(void) {
	
	// Enable IOCON block
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_IOCON_BIT);
	
	// Configure IO Mode and Function of pins 6 and 7 for UART use. FUNC bits (2:0) = 0x01 (RX/TX) respectively.
	IOCON_PIO1_6 = 0x01;		// RX
	IOCON_PIO1_7 = 0x01;		// TX
	
	// Enable power to UART block
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_UART_BIT);
	
	// Set transmission parameters to 8N1 (8 data bits, NO partity, 1 stop bit, DLAB=1)
	UART_LCR = (UART_LCR_WORDLENGTH_08 << UART_LCR_WORDLENGTH_BIT) | (1 << UART_LCR_DLAB_BIT);
	
	// Set baud rate to 115200 kb/s @ UART_CLK of 12Mhz  (DLM = 0, DLL = 4, DIVADDVAL = 5, and MULVAL = 8)
	// UART_DLM = 0x00;		// Default
	UART_DLL = 0x04;
	UART_FDR = 0x85;		// FDR = (MULVAL << 4 ) | DIVADDVAL
	
	// Turn off DLAB
	UART_LCR ^= (1 << UART_LCR_DLAB_BIT);
	
	// Enable UART_PCKL devider to supply clock to the baud generator
	UARTCLKDIV = 0x01;
}

void UART_write(uint8_t *byte, int count) {
		
	for (int i = 0; i < count; i++) {
	
		// Send a byte from buffer
		UART_THR = *byte++;
		
		// Wait while THR contains data (until empty)
		while( (UART_LSR & (1 << UART_LSR_THRE_BIT)) == 0 );
	}
	
}

uint8_t UART_read(void) {
	
	// Wait until there is data in RBR
	while( (UART_LSR & (1 << UART_LSR_RDA_BIT)) == 0);
	
	return UART_RBR;
}