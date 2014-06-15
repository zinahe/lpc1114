#include "LPC1114.h"
#include "GPIO.h"
#include "SysTick.h"
#include "UART.h"

#define GPIO1_5			5

#define IR_STATE_ON		1
#define IR_STATE_OFF	2

volatile uint32_t IR_timer;
volatile uint32_t IR_code;
volatile uint32_t IR_state;
volatile uint32_t IR_bit_count;

void IR_Timer_callback(uint32_t state) {
	
	if (IR_state == IR_STATE_ON) {
	
		IR_timer++;
	} else {
	
		// IR pin Logical Low (Watch for operator precedence b/n & and ==)
		if ((GPIO1DATA & (1 << GPIO1_5)) == 0)  {
			IR_timer++;
		}
	}
	
}


void main() {

	// Initialize SysTick, GPIO and UART
	GPIO_init();
	SysTick_init();
	UART_init();
	
	// ------------- GPIO Interrupt Configuration ------------------
	// GPIO1_ISENSE defaults to Edge Sensitive trigger

	// GPIO1_IEDGES defaults to GPIO1_IEVENT control
	// Configure both edges trigger
	GPIO1_IEDGES = (1 << GPIO1_5);
		
	// Enable Rising Edge trigger
	//GPIO1_IEVENT = (1 << GPIO1_5);
	
	// Enable GPIO Interrupt
	GPIO1_IENABLE = (1 << GPIO1_5);
	
	// Enable GPIO1 Interrupt in NVIC 
	NVIC_SETENA = (1 << NVIC_GPIO1_BIT);
	// ------------- GPIO Interrupt Configuration ------------------

	// Initilize IR state
	IR_timer = 0;
	IR_bit_count = 0;
	IR_code = 0;
	IR_state = IR_STATE_OFF;
	
	// Create and setup IR timer
	Timer_t IR_timer = { 1, 0, 0, 0, IR_Timer_callback };
	SysTick_run(&IR_timer);	
	
	// Despatch timer callbacks if available; or just wait
	while(1) {
		if (Callback_counter > 0) {
			(Timer_callbacks[--Callback_counter])(0);
		}
	}
	
}

void GPIO1_Handler(void) {

	// GPIO1_IMASKSTAT can be used to identify which pin is causing the interrupt 
	
	// Clear GPIO Edge Trigger Intrrupt Signal
	GPIO1_ICLR = (1 << GPIO1_5);
	
	if (GPIO1DATA & (1 << GPIO1_5)) {
		
		// Rising edge trigger
				
		if (IR_state == IR_STATE_OFF) {
		
			// Check for the first 9ms pulse train during OFF state (Watch the difference beween bitwise & and logical &&)
			if ((IR_timer >= 35) && (IR_timer <= 38)) {
			
				// Enter ON state
				IR_state = IR_STATE_ON;
				
			}
		}
		
	} else {
	
		// Falling edge trigger
			
		if (IR_state == IR_STATE_ON) {
			
			// 2.25ms pulse width = IR Bit 1
			if ((IR_timer >= 8) && (IR_timer <= 10)) {
				IR_code++;
				IR_bit_count++;
				
				if (IR_bit_count < 32) 
					IR_code = (IR_code << 1);
			}
			
			// 1.12ms pulse width = IR Bit 0
			if ((IR_timer >= 3) && (IR_timer <= 6)) {
				IR_bit_count++;
				
				if (IR_bit_count < 32) 
					IR_code = (IR_code << 1);
			}
			
			// 32 IR bits = Complete IR command
			if (IR_bit_count == 32) {
			
				// Payload
				UART_write(&IR_code, 4);
				
				
				if (IR_code == 0x00FDB04F) {
					GPIO1DATA ^= (1 << PIO1_9);
				}
				
				
				// Exit to OFF state
				IR_state = IR_STATE_OFF;
				IR_code = 0;
				IR_bit_count = 0;
				
			}
		}
		
		// Reset timer on all falling edge triggers
		IR_timer = 0;
	}
}

/* IR Buttons/Commands

ON/OFF		0x00FD00FF 
PLAY/PAUSE	0x00FDA05F
>>			0x00FD609F
<< 			0x00FD20DF
Vol+		0x00FD807F
VOL-		0x00FD906F
UP			0x00FD50AF
DOWN		0x00FD10EF 
EQ			0x00FDB04F 
0			0x00FD30CF
1			0x00FD08F7
2			0x00FD8877
3			0x00FD48B7

*/
