#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include "LPC1114.h"

typedef struct Timer {
	uint32_t interval;
	uint32_t counter;
	uint32_t state;
		
	//volatile struct Timer * volatile next;
	struct Timer * next;
	
	//void (* volatile callback)(uint32_t data);
	void (*callback)(uint32_t data);	
	
} Timer_t;

// Delarations
void SysTick_init(void);
void wait(uint32_t time);
void SysTick_run(Timer_t *);

//void SysTick_run(volatile Timer_t * volatile);

#endif