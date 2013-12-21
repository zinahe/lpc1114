#include <stdint.h>
#include "LPC1114.h"

#define FALSE 			0
#define TRUE			1

#define I2C_ADDRESS		0x20
#define I2C_READ    	1
#define I2C_WRITE   	0

// SysTick event counter
volatile uint32_t SysTick_counter;

// I2C wait flags
volatile uint32_t I2C_start_continue;
volatile uint32_t I2C_write_continue;

// I2C data
volatile unsigned char data;

// Function declarations
void GPIO_init(void);
void SysTick_init(void);

void wait(uint32_t time);

void I2C_init(void);
static void I2C_start(unsigned char);
void I2C_write(unsigned char);

int main() {
	GPIO_init();
	
	SysTick_init();
	
	I2C_init();
	
	// I2C_start((I2C_ADDRESS << 1) | I2C_WRITE);
	I2C_start(0x00);
	
	while(1) {
		GPIO1DATA |= (1 << PIO1_9);			// Turn ON
		data = 0x00;						// Load I2C data to transmit
		wait(100);
		GPIO1DATA &= ~(1 << PIO1_9);   		// Turn OFF
		data = 0xFF;						// Load I2C data to transmit
		wait(100);
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
	//  IOCON_PIO1_9 = 0xD0;	

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

void I2C_init(void) {
	
	// Configure I2C Mode and Function for pins PIO0_4 and PIO0_5 (FUNC=001:I2C, I2CMODE=00:Standard/Fast Mode)
	IOCON_PIO0_4 = 0x01;
	IOCON_PIO0_5 = 0x01;
	
	// Enable I2C Clock
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_I2C_BIT);
	
	// Reset I2C Block
	PRESETCTRL |= (1 << PRESETCTRL_I2C_BIT);
	
	// Set I2C Clock Rate (100KHz, I2C_CLK_H + I2C_CLK_L = 120, @12MHz system clock )
	I2C_CLK_H = 0x3C;
	I2C_CLK_L = 0x3C;
		
	// Enable I2C Interface (I2EN=1, SDA and SCL inputs are not ignored any more; )
	I2C_CTRL_SET = (1 << I2C_CTRL_I2EN_BIT);
	
	// Enable I2C Interrupt
	NVIC_SETENA = (1 << NVIC_I2C_BIT);
}

static void I2C_start(unsigned char addr) {

	I2C_start_continue = FALSE;
	
	// Transmit START condition 
	I2C_CTRL_SET = (1 << I2C_CTRL_STA_BIT);
	
	// Wait
	while(!I2C_start_continue);
	
}

/*
void I2C_write(unsigned char data) {
	
	I2C_write_continue = FALSE;
	
	// Load data 
	I2C_DATA = (uint32_t) data;
	
	// Wait
	while(!I2C_write_continue);
}
*/

void I2C_Handler(void) {
	
	//GPIO1DATA |= (1 << PIO1_9);			// Turn ON
	
	switch(I2C_STATUS) {
		case 0x08:				// START condition asserted
			
			// Load I2C slave address 
			I2C_DATA = (I2C_ADDRESS << 1) | I2C_WRITE;
			
			// Clear SI and STA bits
			I2C_CTRL_CLR = (1 << I2C_CTRL_STA_BIT) | (1 << I2C_CTRL_SI_BIT);
			
			break;
		case 0x18:				// Slave ADDR : WRITE sent + ACK received
		
			// Flag to continue
			I2C_start_continue = TRUE;
			
			// Clear the SI bit
			I2C_CTRL_CLR = (1 << I2C_CTRL_SI_BIT);
			
			break;
		case 0x28:				// DATA transmitted
		
			I2C_DATA = data;
			
			// Flag to continue
			I2C_write_continue = TRUE;
			
			// Clear the SI bit
			I2C_CTRL_CLR = (1 << I2C_CTRL_SI_BIT);
			
			break;
	}
}

void SysTick_Handler(void) {
	SysTick_counter++;
}





