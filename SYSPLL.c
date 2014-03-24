#include "LPC1114.h"
#include "SYSPLL.h"

void SYSPLL_init(void) {
	
	// Set Post and Feedback dividers (Input=12Mhz, Output=48Mhz, M=4, MSEL,M-1=3 , P=2)
	SYSPLLCTRL = (0x03 << SYSPLLCTRL_MDIV_BIT) | (SYSPLLCTRL_PDIV_02 << SYSPLLCTRL_PDIV_BIT);
	
	// Power-up PLL block
	PDCONFIG =  (PDCONFIG_SYSPLL_ON << PDCONFIG_SYSPLL_BIT) | (PDCONFIG & ~(1 << PDCONFIG_SYSPLL_BIT));
	
	// Wait for PLL lock 
	while(SYSPLLSTAT != SYSPLLSTAT_LOCKED);
	
	// Select PLL as main clock source
	MAINCLKSEL = MAINCLKSEL_PLLOUT;
	MAINCLKUEN = MAINCLKUEN_OFF;
	MAINCLKUEN = MAINCLKUEN_ON;
	
}

