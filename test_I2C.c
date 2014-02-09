#include <stdint.h>
#include "I2C.h"

#define PCF8574_I2C_ADDRESS		0x22

void main() {
	// I2C data
	uint8_t input, output;
	
	// Initialize I2C block
	I2C_init();
	
	// Read I2C switch input and write it back to I2C led output
	while(1) {
		I2C_read(PCF8574_I2C_ADDRESS, &input, 1);
		output = (uint8_t) (0xDF | (input & 0x08) << 2);
		I2C_write(PCF8574_I2C_ADDRESS, &output, 1);
	}

}