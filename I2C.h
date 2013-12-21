#include <stdint.h>
#include "LPC1114.h"

//#define I2C_ADDRESS		0x20
#define I2C_READ    	1
#define I2C_WRITE   	0

#define FALSE 			0
#define TRUE			1

// I2C Task
typedef struct I2CTask {
	uint32_t address;					// I2C slave address
	uint32_t action;					// I2C_READ, I2C_WRITE
	volatile uint8_t *buffer;
	uint32_t count;
	uint32_t current;
} I2CTask_t;

volatile I2CTask_t *i2c_task;
volatile uint32_t I2C_write_continue;

void I2C_init(void);
void I2C_write(uint8_t address, uint8_t byte);

void I2C_init(void) {
	
	// Enable I2C and IOCON Clocks
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_I2C_BIT) | (1 << SYSAHBCLKCTRL_IOCON_BIT);
	
	// Configure I2C Mode and Function for pins PIO0_4 and PIO0_5 (FUNC=001:I2C, I2CMODE=00:Standard/Fast Mode)
	IOCON_PIO0_4 = 0x01;
	IOCON_PIO0_5 = 0x01;
	
	// Enable I2C Clock
	//SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_I2C_BIT);
	
	// Reset I2C Block
	PRESETCTRL |= (1 << PRESETCTRL_I2C_BIT);
	
	// Set I2C Clock Rate (100KHz, CYCLE_H + CYCLE_L = 120, @12MHz system clock )
	I2C_CLK_H = 0x3C;
	I2C_CLK_L = 0x3C;
		
	// Enable I2C Interface (I2EN=1, SDA and SCL inputs are not ignored any more; )
	I2C_CTRL_SET = (1 << I2C_CTRL_I2EN_BIT);
	
	// Enable I2C Interrupt
	NVIC_SETENA = (1 << NVIC_I2C_BIT);
}

void I2C_write(uint8_t address, uint8_t byte) {
	
	// Create and attach the global object
	// Note: Creation needs to happen on every call to write(), hence volatile
	//volatile I2CTask_t task = { I2C_ADDRESS, I2C_WRITE, &byte, 1, 0 };
	volatile I2CTask_t task = { address, I2C_WRITE, &byte, 1, 0 };		
	i2c_task = &task;
		
	// Reset wait flag
	I2C_write_continue = FALSE;
	
	// Transmit START condition (triggers the start of a hardware state machine)
	I2C_CTRL_SET = (1 << I2C_CTRL_STA_BIT);
	
	// Wait on flag (until all bytes in the buffer are sent)
	while(!I2C_write_continue);
	
}

void I2C_Handler(void) {
	
	switch(I2C_STATUS) {
		case 0x08:				// START condition successful
			
			// Load I2C slave address and data direction (Read/Write)
			I2C_DATA = (i2c_task->address << 1) | i2c_task->action;
			
			// Clear SI and STA bits
			I2C_CTRL_CLR = (1 << I2C_CTRL_STA_BIT) | (1 << I2C_CTRL_SI_BIT);
			
			break;
		case 0x18:				// Slave ADDR : WRITE sent + ACK received. 
		
			// Send first byte from buffer
			I2C_DATA = (uint32_t) i2c_task->buffer[i2c_task->current++];
			
			// Clear the SI bit
			I2C_CTRL_CLR = (1 << I2C_CTRL_SI_BIT);
			
			break;
		case 0x28:				// DATA transmitted
		
				
			if (i2c_task->current < i2c_task->count) {
			
				// Load next available byte from buffer
				I2C_DATA = (uint32_t) i2c_task->buffer[i2c_task->current++];
			} 
			else {
				
				// Flag to continue
				I2C_write_continue = TRUE;
				
				// Transmit STOP condition
				I2C_CTRL_SET = (1 << I2C_CTRL_STO_BIT);
			} 
			
			// Clear the SI bit
			I2C_CTRL_CLR = (1 << I2C_CTRL_SI_BIT);
			
			break;
	}
}