#include <stdint.h>

// LCD I2C Address
#define LCD_I2C_ADDR 0x20

// I2C to HD44780 Mapping
#define LCD_RS				0
#define LCD_DATA 			(1 << LCD_RS)
#define LCD_INSTRUCTION 	0x00

#define LCD_RW				1
#define LCD_READ			(1 << LCD_RW)			
#define LCD_WRITE			0x00

#define LCD_EN				2

#define LCD_BACKLIGHT_PIN	3
#define LCD_BACKLIGHT_ON	(1 << LCD_BACKLIGHT_PIN)
#define LCD_BACKLIGHT_OFF	0x00

#define LCD_D4  			4
#define LCD_D5  			5
#define LCD_D6  			6
#define LCD_D7  			7


// HD44780 Instruction Set
#define CMD_CLEAR	 		0b00000001
#define CMD_HOME	 		0b00000010

#define CMD_FUNCTION_SET 	0b00100000
#define FOUR_BIT         	0b00000000
#define EIGHT_BIT        	0b00010000
#define ONE_LINE	 		0b00000000
#define TWO_LINES	 		0b00001000
#define FIVE_SEVEN_FONT  	0b00000000
#define FIVE_TEN_FONT    	0b00000100

#define CMD_DISPLAY      	0b00001000
#define DISPLAY_ON	 		0b00000100
#define DISPLAY_OFF	 		0b00000000
#define CURSOR_ON	 		0b00000010
#define CURSOR_OFF	 		0b00000000
#define BLINK_ON	 		0b00000001
#define BLINK_OFF	 		0b00000000

#define CMD_ENTRY_MODE   	0b00000100
#define INCREMENT	 		0b00000010
#define DECREMENT	 		0b00000000
#define SHIFT_ON	 		0b00000001
#define SHIFT_OFF        	0b00000000

#define CMD_DDRAM_ADDR		0b10000000

void lcd_init(void) ;

void lcd_send_nibble(uint8_t nibble, uint8_t reg);

void lcd_send_byte(uint8_t nibble, uint8_t reg);

void lcd_set_backlight(uint8_t backlight);

void lcd_clear(void) ;

void lcd_write(char *s) ;

void lcd_goto(uint32_t y, uint32_t x);