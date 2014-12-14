#include "I2C.h"
#include "SysTick.h"
#include "font.h"
#include "oled.h"

const uint8_t init_sequence[] = {
	0x00, 0xAE,			// Display Off (sleep)
	0x00, 0xD5, 0x80,	// Set Clock Divide Ratio [3:0], Oscillator Frequency [7:4] 
	0x00, 0xA8, 0x3F,	// Set Multiplex Ratio (1/64 duty)
	0x00, 0xD3, 0x00,	// Set Display Offset/Shift COM lines UP (0 to 63)
	0x00, 0x40,			// Set Display Start Line/Shift GDRAM content DOWN (0 to 63)
	0x00, 0x8D, 0x14,	// Enable Charge Pump (0x14)
	0x00, 0x20, 0x00,	// Set Addressing mode (00: Horizontal, 01: Vertical, 02: Page) 
	0x00, 0xA1,			// Set Vertical/Segment scan dir. (0xA0: 0 to 127, 0xA1: 127 to 0)
	0x00, 0xC8,			// Set Horizontal/COM scan dir. (0xC0: 0 to 63, 0xC8: 63 to 0)
	0x00, 0xDA, 0x12,	// Set COM pin configuration (Alternating)
	0x00, 0x81, 0xCF,	// Set Display Contrast (256 contrast steps 0x00 to 0xFF)
	0x00, 0xD9, 0xF1,	// Set pre-charge phase 1 time
	0x00, 0xDB, 0x40,	// Set VCOMH detect 
	0x00, 0xA4,			// Set display resume (A4: display DDRAM, A5: All on)
	0x00, 0xA6,			// Normal display (A6: Normal, A7: Inverse) 
	0x00, 0xAF			// Display On (activate)
};

static uint8_t goto_sequence[] = {
	// Important to set the page/row before the column
	0x00, 0x22, 0x00, 0x07,		// Set page line start/end address
	0x00, 0x21, 0x00, 0xFF		// Set column start/end address
};

static uint8_t command_sequence[] = { 
	0x00, 0x00		
};

void oled_init() {

	// Initialize I2C and SysTick
	I2C_init();
	SysTick_init();
	
	wait(1000);

	// Initialize OLED
	I2C_write(OLED_I2C_ADDRESS, init_sequence, 41);
}

void oled_write(char *s) {
	uint32_t i = 0;
	
	while(s[i] != '\0') {
		I2C_write(OLED_I2C_ADDRESS, fonts[s[i] - FONT_OFFSET], FONT_WIDTH + 1);
		I2C_write(OLED_I2C_ADDRESS, gap, GAP_WIDTH + 1);
		
		i++;
	}
}

void oled_goto(uint8_t x, uint8_t y) {
	goto_sequence[6] = x * (FONT_WIDTH + GAP_WIDTH);
	goto_sequence[2] = y;

	I2C_write(OLED_I2C_ADDRESS, goto_sequence, 8);
}

void oled_clear() {

	// Go to 0, 0
	oled_goto(0, 0);

	// Write 16x7 empty spaces
	for (int i = 0; i < (OLED_WIDTH/(FONT_WIDTH + GAP_WIDTH) * OLED_HEIGHT/FONT_HEIGHT); i++)
		oled_write(" ");

	// Go to 0, 0
	//oled_goto(0, 0);
}

void oled_display_off() {

	command_sequence[1] = 0xAE;
	I2C_write(OLED_I2C_ADDRESS, command_sequence, 2);
}

void oled_display_on() {

	command_sequence[1] = 0xAF;
	I2C_write(OLED_I2C_ADDRESS, command_sequence, 2);
}

