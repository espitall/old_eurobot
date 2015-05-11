#include <avr/io.h>
#include "servo.h"

void servoInit(void)
{
  PORTD.DIRSET = (1 << 0) | (1 << 1);
  PORTD.OUTSET = (1 << 0) | (1 << 1);
  PORTE.DIRSET = (1 << 2) | (1 << 3);
  PORTE.OUTSET = (1 << 2) | (1 << 3);
  TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
  TCD0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC_WGMODE_SS_gc;
  TCD0.PER = 20000;
  TCD0.CCA = 0;
  TCD0.CCB = 0;
  TCE0.CTRLA = TC_CLKSEL_DIV64_gc;
  TCE0.CTRLB = TC0_CCCEN_bm | TC0_CCDEN_bm | TC_WGMODE_SS_gc;
  TCE0.PER = 20000;
  TCE0.CCA = 0;
  TCE0.CCB = 0;

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

    case SERVO_SLIDER_RIGHT:
      TCE0.CCC = value;
      break;

    case SERVO_SLIDER_LEFT:
      TCE0.CCD = value;
      break;

  }
}
