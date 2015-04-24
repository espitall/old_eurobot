#include "step.h"

#if defined(STEP_SLAVE)

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int regaddr;
static volatile uint8_t * regptr;
static volatile uint8_t * nextregptr;
static volatile uint8_t nextByte;
static volatile uint8_t firstByte;

static const uint16_t board_id = STEP_ID;
static volatile uint16_t status = STEP_STATUS_BUSY;
static volatile uint8_t command = STEP_ACTION_WAIT;

ISR(PORTD_INT0_vect)
{
  if(nextregptr != NULL)
  {
    SPID.DATA = nextregptr[0];
    nextByte = nextregptr[1];
  }
  else
  {
    SPID.DATA = 0x42;
    nextByte = 0x00;
  }
  regptr = nextregptr;
  regaddr = 0;
}

ISR(SPID_INT_vect)
{
  volatile uint8_t read = SPID.DATA;
  SPID.DATA = nextByte;

  if(regaddr == 0)
  {
    firstByte = read;
    switch(read & 0x7F)
    {
      case STEP_BOARD_ID:
        nextregptr = (uint8_t *)&board_id;
        break;

      case STEP_COMMAND:
        nextregptr = (uint8_t *)&status;
        break;
    }
  }
  else
  {
    if(!(firstByte & 0x80))
    {
      switch(firstByte & 0x7F)
      {
        case STEP_COMMAND:
          {
            status = STEP_STATUS_BUSY;
            uint8_t * ptr = (uint8_t *)&command;
            ptr[regaddr - 1] = read;
          }
          break;
      }
    }
  }

  regaddr += 1;
  nextByte = regptr[regaddr + 1];
}

void stepInit(void)
{
  //init vars
  regaddr = 0;
  regptr = NULL;
  nextregptr = NULL;
  firstByte = 0xff;

  // port
  PORTD.DIRSET = 1 << 6;
  PORTD.PIN4CTRL = PORT_ISC_FALLING_gc;
  PORTD.INT0MASK = (1 << 4);
  PORTD.INTCTRL = PORT_INT0LVL_HI_gc;
  
  //init spi
  SPID.CTRL = SPI_ENABLE_bm | SPI_MODE_3_gc;
  SPID.INTCTRL = SPI_INTLVL_HI_gc;
}

step_action_t stepGetCommand(void)
{
  return command;
}

void stepSetReady(void)
{
  command = STEP_ACTION_WAIT;
  status = 0;
}

#endif

