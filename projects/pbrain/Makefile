## Makefile script from avarix

## Project configuration

SRCS = $(filter-out avoidance%, $(wildcard *.c))
ASRCS =
TARGET = main
MODULES = xmega/clock com FreeRTOS

## Target configuration
MCU = atxmega128a1

## Build configuration

OPT = s
# link against math library (may be implied by some modules)
MATH_LIB = yes

CFLAGS += -g -Wall -Wextra -Werror
LDFLAGS += -g

include ../../mk/project.mk

# Program configuration
AVRDUDE = avrdude
AVRDUDE_PROGRAMMER = avrisp2
AVRDUDE_PORT = usb	   # programmer connected to serial device
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

