#ifndef USIR_H
#define USIR_H

#include <stdint.h>
#include <config.h>

#define USIR_ID 0x1989

typedef enum
{
  USIR_BOARD_ID,

  USIR_IR_CH0,
  USIR_IR_CH1,
  USIR_IR_CH2,
  USIR_IR_CH3,

  USIR_US_CH0,
  USIR_US_CH1,
  USIR_US_CH2,
  USIR_US_CH3,

} usir_registers_t;

void usirInit(void);
void usirSetIRRaw(int channel, uint16_t raw);
void usirSetUSRaw(int channel, uint16_t raw);

#if defined(USIR_MASTER)

uint16_t usirGetDistancemm(int channel);

#elif defined(USIR_SLAVE)
#else
# error "No master/slave usir configuration selected"
#endif

#endif// USIR_H
