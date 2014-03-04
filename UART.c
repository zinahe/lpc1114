#include <stdint.h>
#include "LPC1114.h"
#include "UART.h"
#include "GPIO.h"

void (*UART_callback)(uint8_t);

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
	
	// Enable UART Interrupt (NVIC_SETENA is a set only register. No need for a RMW operation)
	NVIC_SETENA = (1 << NVIC_UART_BIT);
}

void UART_write(uint8_t *byte, int count) {
		
	for (int i = 0; i < count; i++) {
	
		// Send a byte from buffer
		UART_THR = *byte++;
		
		// Wait while THR contains data (until empty)
		while( (UART_LSR & (1 << UART_LSR_THRE_BIT)) == 0 );
	}
	
}

uint8_t UART_read(void (*callback)(uint8_t)) {

	// No callback
	if (callback == 0) {
	
		// Wait until there is data in RBR
		while( (UART_LSR & (1 << UART_LSR_RDA_BIT)) == 0);
	
		return UART_RBR;
	
	} else {
	
		// Attach callback function
		UART_callback = callback;
	
		// Enable UART RX Data Available (RDA) interrupt
		UART_IER = (1 << UART_IER_RDA_BIT);
	}
}

void UART_Handler(void) {

	// Identify and process particular interrupt types
	switch(UART_IIR & UART_IIR_INTID_MASK) {
	
		case (UART_IIR_INTID_LSR << UART_IIR_INTID_BIT):		// Status Change
		
			break;
		
		case (UART_IIR_INTID_RDA << UART_IIR_INTID_BIT):		// RX Data Available
		
			// Call UART read callback function if not NULL
			if (UART_callback)
				UART_callback(UART_RBR);
			
			break;
	}
	
	// Disable UART interrupt
	//UART_IER ^= (1 << UART_IER_RDA_BIT);
}

/* 
uint8_t UART_read(void) {
	
	// Wait until there is data in RBR
	while( (UART_LSR & (1 << UART_LSR_RDA_BIT)) == 0);
	
	return UART_RBR;
}

*/

// Note: How to check if interrupt is happening
	// Wait for interrupt to happen
	//while( (UART_IIR & (1 << UART_IIR_INT_BIT)) == 1 );