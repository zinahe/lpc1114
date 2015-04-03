#include "LPC1114.h"
#include "GPIO.h"

void GPIO_init(void) {
	
	// Enable Clock to IOCON Configuration Block (pp. 30)
	SYSAHBCLKCTRL |= (1 << SYSAHBCLKCTRL_IOCON_BIT);
		
	//  Configure PIO1_9 as General Purpose IO (Reset value = 0xD0)
	IOCON_PIO1_9 = 0xC0;		// No pull-up resistor
	
	//  Configure PIO1_8 as General Purpose IO (Reset value = 0xD0)
	IOCON_PIO1_8 = 0xC0;		// No pull-up resistor

	//  Configure PIO1_9 as Output (pp.188)
	GPIO1DIR |= (1 << PIO1_9); 		//0x200;
	
	//  Configure PIO1_8 as Output (pp.188)
	GPIO1DIR |= (1 << PIO1_8); 		//0x200;
}
