#include <stdint.h>
#include "LPC1114.h"

#define I2C_ADDRESS		0x20
#define I2C_READ    	1
#define I2C_WRITE   	0

#define FALSE 			0
#define TRUE			1

// SysTick event counter
volatile uint32_t SysTick_counter;
void SysTick_init(void);
void wait(uint32_t time);

// Function declarations
void GPIO_init(void);


// I2C Task
typedef struct I2CTask {
	uint32_t address;					// I2C slave address
	uint32_t action;					// I2C_READ, I2C_WRITE
	volatile unsigned char *buffer;
	uint32_t count;
	uint32_t current;
} I2CTask_t;

volatile I2CTask_t *i2c_task;
volatile uint32_t I2C_write_continue;

void I2C_init(void);
void I2C_write(I2CTask_t *);

int main() {

	unsigned char buffer[1];
	I2CTask_t task = { I2C_ADDRESS, I2C_WRITE, buffer, 1, 0 };
		
	// Initialize peripherals
	GPIO_init();
	SysTick_init();
	I2C_init();
	
	// Loop
	while(1) {
		GPIO1DATA |= (1 << PIO1_9);			// LED ON
		task.buffer[0] = 0xFF;
		I2C_write(&task);
		wait(150);
		
		GPIO1DATA &= ~(1 << PIO1_9);   		// LED OFF
		task.buffer[0] = 0x00;
		I2C_write(&task);
		wait(150);
	} 
	
}

void wait(uint32_t time) {
	
	SysTick_counter = 0;
	while (SysTick_counter < time) {
		// Wait until SysTick increments counter enough times
	}
}

void GPIO_init(void) {
	
	// Enable Clock to IOCON Configuration Block (pp. 30)
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_IOCON_BIT);
		
	//  Configure PIO1_9 as General Purpose IO (Reset value = 0xD0)
	IOCON_PIO1_9 = 0xC0;		// No pull-up resistor

	//  Configure PIO1_9 as Output (pp.188)
	GPIO1DIR = (1 << PIO1_9); 		//0x200;
}

void SysTick_init(void) {
	
	// SysTick Reload Value Register, 10ms interval using the Internal RC @12Mhz (pp. 403)
	SYSTICK_RVR = 0x1D4BF;   
	
	// Clear Current Value Register (pp. 403)
	SYSTICK_CVR = 0; 						
	
	// Enable SysTick, on CPU clock and Enable SysTick interrupt (pp. 402)
	SYSTICK_CSR |= (1 << SYSTICK_CLKSOURCE_BIT) | (1 << SYSTICK_TICKINT_BIT) | (1 << SYSTICK_ENABLE_BIT);
}

void SysTick_Handler(void) {
	SysTick_counter++;
}

void I2C_init(void) {
	
	// Configure I2C Mode and Function for pins PIO0_4 and PIO0_5 (FUNC=001:I2C, I2CMODE=00:Standard/Fast Mode)
	IOCON_PIO0_4 = 0x01;
	IOCON_PIO0_5 = 0x01;
	
	// Enable I2C Clock
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_I2C_BIT);
	
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

void I2C_write(I2CTask_t *task) {
	
	I2C_write_continue = FALSE;
	
	// Reset current counter and attach global object
	i2c_task = task;
	i2c_task->current = 0;
		
	// Transmit START condition (triggers the start of a hardware state machine)
	I2C_CTRL_SET = (1 << I2C_CTRL_STA_BIT);
	
	// Wait until all bytes in the buffer are sent
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




