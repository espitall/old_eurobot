#include <avr/io.h>
#include <xmega/pwm/pwm.h>
#include "motors.h"
#include "config.h"

static pwm_t pwms;

void motors_init(void)
{
  //init pwm
  PORTE.DIRSET = (1 << 2) | (1 << 3);
  PORTH.DIRSET = 0x0F;
  pwm_init(&pwms,(TC0_t *)&TCE0,TC_CLKSEL_DIV1_gc, TC0_CCCEN_bm | TC0_CCDEN_bm);
  pwm_set_period(&pwms, MOTORS_PWM_PERIOD);

  //put robot in a safety state
  motors_set_consign(0, 0);
}

void motors_set_consign(int32_t left, int32_t right)
{
  //compute max and min
  if(left > MOTORS_PWM_PERIOD)
  {
    left = MOTORS_PWM_PERIOD;
  }
  if(left < -MOTORS_PWM_PERIOD)
  {
    left = -MOTORS_PWM_PERIOD;
  }
  if(right > MOTORS_PWM_PERIOD)
  {
    right = MOTORS_PWM_PERIOD;
  }
  if(right < -MOTORS_PWM_PERIOD)
  {
    right = -MOTORS_PWM_PERIOD;
  }

#ifdef MOTOR_0_INVERT
  if(MOTOR_0 < 0)
#else
  if(MOTOR_0 >= 0)
#endif
  {
    PORTH.OUTCLR = (1 << 0);
    PORTH.OUTSET = (1 << 1);
  }
  else
  {
    PORTH.OUTSET = (1 << 0);
    PORTH.OUTCLR = (1 << 1);
  }

#ifdef MOTOR_1_INVERT
  if(MOTOR_1 < 0)
#else
  if(MOTOR_1 >= 0)
#endif
  {
    PORTH.OUTCLR = (1 << 2);
    PORTH.OUTSET = (1 << 3);
  }
  else
  {
    PORTH.OUTSET = (1 << 2);
    PORTH.OUTCLR = (1 << 3);
  }

  //set pwm
  pwm_set_output(&pwms, 'C', MOTOR_0);
  pwm_set_output(&pwms, 'D', MOTOR_1);
}
