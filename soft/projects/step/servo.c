#include <avr/io.h>
#include "servo.h"

void servoInit(void)
{
  PORTD.DIRSET = (1 << 0) | (1 << 1);
  PORTD.OUTSET = (1 << 0) | (1 << 1);
  TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
  TCD0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC_WGMODE_SS_gc;
  TCD0.PER = 20000;
  TCD0.CCA = 0;
  TCD0.CCB = 0;
}

void servoSet(servo_t id, uint16_t value)
{
  switch(id)
  {
    case SERVO_CLAMP_LEFT:
      TCD0.CCA = value;
      break;

    case SERVO_CLAMP_RIGHT:
      TCD0.CCB = value;
      break;
  }
}
