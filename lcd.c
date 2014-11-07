#include <stdint.h>
#include "LPC1114.h"
#include "SysTick.h"
#include "I2C.h"
#include "lcd.h"		

static uint8_t lcd_backlight;

static void lcd_send_nibble(uint8_t nibble, uint8_t reg) {

	nibble &= 0xF0;
	nibble |= (lcd_backlight | reg);
	I2C_write(LCD_I2C_ADDR, &nibble, 1);

	// Set ENABLE high
	nibble |= (1 << LCD_EN);		
	I2C_write(LCD_I2C_ADDR, &nibble, 1);

	// Pull ENABLE low
	nibble &= ~(1 << LCD_EN);
	I2C_write(LCD_I2C_ADDR, &nibble, 1);
}

static void lcd_send_byte(uint8_t nibble, uint8_t reg) {

	// Send 4 most significant bits
	lcd_send_nibble(nibble, reg);

	// Send 4 least significant bits
	lcd_send_nibble(nibble << 4, reg);
}

void lcd_init(void) {

 	// Initialize I2C and SysTick
	I2C_init();
	SysTick_init();

	// HD44780 4bit initialization sequence
	// Wait 60ms 
	wait(240);
	lcd_send_nibble(CMD_FUNCTION_SET | EIGHT_BIT, LCD_INSTRUCTION);	
	
	// Wait 4ms
	wait(16);
	lcd_send_nibble(CMD_FUNCTION_SET | EIGHT_BIT, LCD_INSTRUCTION);		
	lcd_send_nibble(CMD_FUNCTION_SET | EIGHT_BIT, LCD_INSTRUCTION);		
	lcd_send_nibble(CMD_FUNCTION_SET | FOUR_BIT,  LCD_INSTRUCTION);		
	
	// HD44780 Function Set 
	lcd_send_byte(CMD_FUNCTION_SET | FOUR_BIT | TWO_LINES | FIVE_SEVEN_FONT, LCD_INSTRUCTION);

	// HD44780 Display OFF Command
	lcd_send_byte(CMD_DISPLAY | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF, LCD_INSTRUCTION);

	// HD44780 Clear Display
	lcd_send_byte(CMD_CLEAR, LCD_INSTRUCTION);
	
	// Wait 4ms
	wait(16);

	// HD44780 Set Entry Mode
	lcd_send_byte(CMD_ENTRY_MODE | INCREMENT | SHIFT_OFF, LCD_INSTRUCTION);

	// HD44780 Display ON
	lcd_send_byte(CMD_DISPLAY | DISPLAY_ON | CURSOR_ON | BLINK_OFF, LCD_INSTRUCTION);
}

void lcd_clear(void) {

	// Send CLEAR instruction
	lcd_send_byte(CMD_CLEAR, LCD_INSTRUCTION);

	// Wait 4ms
	wait(16);
}

void lcd_write(char *s) {
	uint32_t i = 0;

	while(s[i] != '\0') {
		lcd_send_byte(s[i], LCD_DATA);
		i++;
	}
}

void lcd_goto(uint32_t y, uint32_t x) {
	uint8_t ddram_addr;

	ddram_addr = 0;
	
	if (y == 0) {
		ddram_addr = CMD_DDRAM_ADDR + x;
	} else if (y == 1) {
		ddram_addr = CMD_DDRAM_ADDR + x + 0x40;
	} else if (y == 2) {
		ddram_addr = CMD_DDRAM_ADDR + x + 0x14;
	} else if (y == 3) {
		ddram_addr = CMD_DDRAM_ADDR + x + 0x54;
	}

	lcd_send_byte(ddram_addr, LCD_INSTRUCTION);
}

/* void lcd_set_backlight(uint8_t backlight) {

	lcd_backlight = backlight;

	I2C_write(LCD_I2C_ADDR, &backlight, 1);
	
} */

void lcd_backlight_ON() {

	lcd_backlight = LCD_BACKLIGHT_ON;

	I2C_write(LCD_I2C_ADDR, &lcd_backlight, 1);
}

void lcd_backlight_OFF() {

	lcd_backlight = LCD_BACKLIGHT_OFF;

	I2C_write(LCD_I2C_ADDR, &lcd_backlight, 1);
}

