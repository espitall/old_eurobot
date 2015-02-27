#ifndef USIR_H
#define USIR_H

#include <stdint.h>
#include <config.h>

typedef enum
{
  USIR_IR_CH0,
  USIR_IR_CH1,

  USIR_US_CH0,
  USIR_US_CH1,

} usir_registers_t;

void usirInit(void);

#if defined(USIR_MASTER)

uint16_t usirGetDistancemm(int channel);

#elif defined(USIR_SLAVE)
#else
# error "No master/slave usir configuration selected"
#endif

#endif// USIR_H
