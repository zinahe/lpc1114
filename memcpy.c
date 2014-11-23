#include <stdint.h>

typedef struct Sample1 {
	uint32_t x;					
} Sample1_t;

typedef struct Sample2 {
	uint32_t x;					
	uint32_t y;					
} Sample2_t;

typedef struct Sample3 {
	uint32_t x;					
	uint32_t y;					
	uint32_t z;					
} Sample3_t;

typedef struct Sample4 {
	uint32_t x;					
	uint32_t y;					
	uint32_t z;	
	uint32_t buffer;	
} Sample4_t;

typedef struct Sample5 {
	uint32_t x;					
	uint32_t y;					
	uint32_t z;
	uint32_t buffer;
	uint32_t current;
} Sample5_t;

typedef struct Sample6 {
	uint32_t x;					
	uint32_t y;					
	uint32_t z;
	uint32_t buffer;
	uint32_t current;
	uint32_t state;
} Sample6_t;

void func() {
	volatile uint32_t _x = 5;

	// Any one of these these will generate a memcpy
	//volatile Sample2_t s = { _x, 10 };					
	//volatile Sample4_t s = { _x, 10, 15, 20 };
	//volatile Sample6_t s = { _x, 10, 15, 20, 25, 30 };
	
	
	// None of these will generate a memcpy
	volatile Sample1_t s = { 10 };
	//volatile Sample1_t s = { _x };
	
	//volatile Sample2_t s = { 5, 10 };						// Even number of parameters : ALL integer literals
	
	//volatile Sample3_t s = { 5, 10, 15 };
	//volatile Sample3_t s = { _x, 10, 15 };
	
	//volatile Sample4_t s = {  5, 10, 15, 20 };            // Even number of parameters : ALL integer literals
	
	//volatile Sample5_t s = {  5, 10, 15, 20, 25 };
	//volatile Sample5_t s = {  _x, 10, 15, 20, 25 };
	
	//volatile Sample6_t s = {  5, 10, 15, 20, 25, 0 };     // Even number of parameters : ALL integer literals
}


int main(void) {
	
	func();
	
	while(1);
}
