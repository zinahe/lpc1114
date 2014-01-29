#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_init(void); 
void UART_write(uint8_t*, int);
uint8_t UART_read(void);

#endif /* UART_H */



