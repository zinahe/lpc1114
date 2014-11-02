#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

typedef struct Timer {
	uint32_t interval;
	uint32_t counter;
	uint32_t state;
		
	struct Timer * next;
	
	void (*callback)(uint32_t);	
	
} Timer_t;

// Delarations

void SysTick_add(Timer_t *);
void SysTick_run(void);
void SysTick_init(void);
void wait(uint32_t time);

//extern void (* volatile Timer_callbacks[])(uint32_t);
//extern volatile uint32_t Callback_counter;

#endif /* SYSTICK_H */