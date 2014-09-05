## Makefile script from avarix

## Project configuration

SRCS = $(filter-out avoidance%, $(wildcard *.c))
ASRCS =
TARGET = main
MODULES = position asserv encoder_magnetic FreeRTOS

## Target configuration
HOST = arm
MCU = stm32f429

## Build configuration

OPT = s
# link against math library (may be implied by some modules)
MATH_LIB = yes

CFLAGS += -g -Wall -Wextra -Werror
LDFLAGS += -g

include ../../mk/project.mk

# Program configuration

prog: $(TARGET).bin
	st-flash write $(TARGET).bin 0x8000000
