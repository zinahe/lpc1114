# Sources and targets
TARGET			= main

#SOURCE			= startup.c test_I2C.c I2C.c
SOURCE			= startup.c test_SysTick.c GPIO.c SysTick.c 
#SOURCE			= startup.c test_lcd.c SysTick.c I2C.c lcd.c
#SOURCE			= startup.c test_SYSPLL.c UART.c SysTick.c lcd.c I2C.c GPIO.c SYSPLL.c
#SOURCE			= startup.c IR.c GPIO.c UART.c SysTick.c 
#SOURCE			= startup.c test_Stepper.c UART.c SysTick.c lcd.c I2C.c GPIO.c
#SOURCE			= startup.c test_UART_TX.c UART.c SysTick.c GPIO.c I2C.c

#SOURCE			= startup.c main.c lcd.c SysTick.c I2C.c GPIO.c

# Processor/Microcontroller family
MCU				= cortex-m0

# Tools
CC 				= arm-none-eabi-gcc
OBJCOPY 		= arm-none-eabi-objcopy
OBJDUMP			= arm-none-eabi-objdump
RM 				= del
ECHO			= echo

# C Compiler Optons
CFLAGS=-mthumb # 	           		  Using the Thumb Instruction Set
CFLAGS+= -mcpu=$(MCU) #			      The MCU Family
CFLAGS+= -Os # 						  Compile with Size Optimizations
#CFLAGS+= -g #						  Generate debugging info
#CFLAGS+= -v #						  Generate verbose output
CFLAGS+= -ffunction-sections # 		  Create a separate function section
CFLAGS+= -fdata-sections # 			  Create a separate data section
CFLAGS+= -std=c99 # 				  Comply with C99
CFLAGS+= -Wall # 					  Enable All Warnings 
CFLAGS+= -fno-common #				  Disable COMMON sections
CFLAGS+= -Wa,-adhlns=$(<:%.c=%.lst) # Generate assembly files

# Linker Options
LDFLAGS=-Wl,--gc-sections # 						Linker to ignore sections that aren't used.
LDFLAGS+= -Wl,-Map,$(TARGET).map #					Generate memory map file
LDFLAGS+= -Wl,-T,".\$(TARGET).ld" # 	Path to Linker Script
LDFLAGS+= -nostdlib
# LDFLAGS+= -nostartfiles

# Define object and assembly list files
OBJ = $(SOURCE:%.c=%.o)
LST = $(SOURCE:%.c=%.lst)

.PHONY: all clean disasm

all: $(TARGET).hex
		
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)
	
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	$(RM) $(OBJ)
	$(RM) $(TARGET).hex $(TARGET).elf $(TARGET).map $(TARGET).lss $(LST)
	
disasm: $(TARGET).lss

$(TARGET).lss: $(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

	
#	arm-none-eabi-objcopy -O ihex hello_world.elf hello_world.hex
#	arm-none-eabi-gcc -Wl,--gc-sections -Wl,-Map,hello_world.map -Wl,-T,"..\startup.ld" -o hello_world.elf hello_world.o startup.o
#   arm-none-eabi-gcc -Os -ffunction-sections -fdata-sections -Wall -c -mcpu=cortex-m0 -mthumb -o hello_world.o hello_world.c

#   [How to view a particular section]
# 	arm-none-eabi-objdump -s .vectors hello_world.elf

#	[How to view the symbol table]
#	arm-none-eabi-objdump -t hello_world.elf
#	arm-none-eabi-nm -n hello_world.elf

