#ifndef OLED_H
#define OLED_H

#define OLED_I2C_ADDRESS 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

void oled_init(void); 
void oled_write(char *); 
void oled_goto(uint8_t, uint8_t);
void oled_clear(void); 
void oled_display_off(void);
void oled_display_on(void);

#endif /* OLED_H */
