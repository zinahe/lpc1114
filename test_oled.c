#include "SysTick.h"
#include <stdint.h>
#include "oled.h"

int main() {

	oled_init();

	oled_clear();

	oled_write("Data const'd");
	
	oled_goto(1, 1);
	oled_write("Next Line.");
	
	oled_goto(2, 2);
	oled_write("Third Line.");
	
	wait(9000);

	oled_display_off();
	
	wait(9000);

	oled_display_on();
	while(1);
}


