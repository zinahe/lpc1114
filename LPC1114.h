// Memory Mapped Peripheral Control Registers must be accessed as volatile to avoid compiler optimization
#define  MMIO(addr) 					(*(volatile int*) addr)

// Peripheral RESET Register SCB) Registers
#define PRESETCTRL						MMIO(0x40048004)
#define PRESETCTRL_I2C_BIT				1

//  SYSCON Registers (SCB)
#define SYSAHBCLKCTRL  					MMIO(0x40048080)
#define SYSAHBCLKCTRL_IOCON_BIT 		16
#define SYSAHBCLKCTRL_I2C_BIT 			5

// IOCON - I2C 
#define IOCON_PIO0_4					MMIO(0x40044030)
#define IOCON_PIO0_5					MMIO(0x40044034)

// IOCON - GPIO
#define IOCON_PIO0_8   					MMIO(0x40044060)
#define IOCON_PIO1_9   					MMIO(0x40044038)

// GPIO Registers
#define GPIO1DIR      					MMIO(0x50018000)
#define GPIO1DATA      					MMIO(0x50013FFC)

// SysTick Registers (WORD ADDR)
#define SYSTICK_CSR						MMIO(0xE000E010) 
#define SYSTICK_RVR						MMIO(0xE000E014) 
#define SYSTICK_CVR						MMIO(0xE000E018) 

// SYSTICK_CLKSOURCE : Use CPU (core)'s clock
// SYSTICK_TICKINT 	 : SysTick interrupt is activated on NVIC
// SYSTICK_ENABLE 	 : Enable SysTick
#define SYSTICK_CLKSOURCE_BIT 			2
#define SYSTICK_TICKINT_BIT  			1
#define SYSTICK_ENABLE_BIT 				0

// NVIC Registers (WORD ADDR)
#define NVIC_SETENA						MMIO(0xE000E100)
#define NVIC_CLRENA						MMIO(0xE000E180)

// NVIC IRQ I2C
#define NVIC_I2C_BIT					15

// I2C Peripheral REgisters
#define I2C_CTRL_SET					MMIO(0x40000000)
#define I2C_STATUS						MMIO(0x40000004)
#define I2C_DATA						MMIO(0x40000008)
#define I2C_CLK_H						MMIO(0x40000010)
#define I2C_CLK_L						MMIO(0x40000014)
#define I2C_CTRL_CLR					MMIO(0x40000018)

#define I2C_CTRL_AA_BIT					2
#define I2C_CTRL_SI_BIT					3
#define I2C_CTRL_STO_BIT				4
#define I2C_CTRL_STA_BIT				5
#define I2C_CTRL_I2EN_BIT				6

// LED pin
#define PIO1_9 							9



