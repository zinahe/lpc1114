#include "I2C.h"

#define PCF8574_I2C_ADDRESS		0x22
#define I2C_SWITCH				0b00001000
#define I2C_LED					0b00100000

void main() {
	// I2C data
	uint8_t input, output;
	
	// Initialize I2C block
	I2C_init();
	
	// Read I2C switch input and write it back to I2C led output
	while(1) {
		I2C_read(PCF8574_I2C_ADDRESS, &input, 1);
		output = (uint8_t) (~I2C_LED | (input & I2C_SWITCH) << 2);		// Shift left by 2 bits from Switch pin at P3 to Led pin at P5
		I2C_write(PCF8574_I2C_ADDRESS, &output, 1);
	}

}