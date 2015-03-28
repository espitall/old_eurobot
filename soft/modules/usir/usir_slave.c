#include "usir.h"

#if defined(USIR_SLAVE)

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int regaddr;
static volatile uint8_t * regptr;
static volatile uint8_t * nextregptr;
static volatile uint8_t nextByte;

static volatile uint16_t dist_ir[8];
static volatile uint16_t dist_us[4];

ISR(PORTD_INT0_vect)
{
  regaddr = 0;
  regptr = nextregptr;
  if(regptr != NULL)
  {
    SPID.DATA = regptr[0];
    nextByte = regptr[1];
  }
  else
  {
    SPID.DATA = 0x00;
    nextByte = 0x00;
  }
}

ISR(SPID_INT_vect)
{
  volatile uint8_t read = SPID.DATA;
  SPID.DATA = nextByte;

  if(regaddr == 0)
  {
    switch(read & 0x7F)
    {
      case USIR_IR_CH0:
        nextregptr = (uint8_t *)&dist_ir[0];
        break;

      case USIR_IR_CH1:
        nextregptr = (uint8_t *)&dist_ir[1];
        break;

      case USIR_IR_CH2:
        nextregptr = (uint8_t *)&dist_ir[2];
        break;

      case USIR_IR_CH3:
        nextregptr = (uint8_t *)&dist_ir[3];
        break;

      case USIR_US_CH0:
        nextregptr = (uint8_t *)&dist_us[0];
        break;
    }
  }

  regaddr += 1;
  nextByte = regptr[regaddr + 1];
}

void usirInit(void)
{
  int i;

  //init vars
  regaddr = 0;
  regptr = NULL;
  nextregptr = NULL;
  for(i = 0; i < 8; i += 1)
  {
    dist_ir[i] = 42;
  }
  for(i = 0; i < 4; i += 1)
  {
    dist_us[i] = 1000;
  }

   // port
  PORTD.DIRSET = 1 << 6;
  PORTD.PIN4CTRL = PORT_ISC_FALLING_gc;
  PORTD.INT0MASK = (1 << 4);
  PORTD.INTCTRL = PORT_INT0LVL_HI_gc;
  
 //init spi
  SPID.CTRL = SPI_ENABLE_bm | SPI_MODE_3_gc;
  SPID.INTCTRL = SPI_INTLVL_HI_gc;
}

void usirSetIRRaw(int channel, uint16_t raw)
{
  cli();
  dist_ir[channel] = raw;
  sei();
}

#endif
