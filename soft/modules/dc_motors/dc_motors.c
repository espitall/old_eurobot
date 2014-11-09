#include <ch.h>
#include <hal.h>
#include "dc_motors.h"

static PWMConfig pwmcfg = {
  .frequency = 40000000, /* 40Mhz PWM clock frequency */
  .period = 2000, /* PWM frequency 20kHz */
  .callback = NULL,
  .channels = {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
  },
  .cr2 = 0,
 .dier = 0,
};

void dcmInit(void)
{
   pwmStart(&PWM_MOTORS, &pwmcfg);

   palSetPad(GPIOC, GPIOC_RST_MX);

   pwmEnableChannel(&PWM_MOTORS, 0, 1000); 
   pwmEnableChannel(&PWM_MOTORS, 1, 1000); 
   pwmEnableChannel(&PWM_MOTORS, 2, 1000); 
   pwmEnableChannel(&PWM_MOTORS, 3, 1000); 
}

void dcmSetWidth(unsigned int channel, int width)
{
  unsigned int pwm_channel;

  switch(channel)
  {
    case 0:
      pwm_channel = 1;
      break;

    case 1:
      pwm_channel = 2;
      break;

    default:
      return;
  }

  if(width < 0)
  {
    width = -width;

    switch(channel)
    {
      case 0:
        palClearPad(GPIOC, GPIOC_DIR_M0);
        break;

      case 1:
        palClearPad(GPIOC, GPIOC_DIR_M1);
        break;
    }
  }
  else
  {
    width = 10000 - width;
    switch(channel)
    {
      case 0:
        palSetPad(GPIOC, GPIOC_DIR_M0);
        break;

      case 1:
        palSetPad(GPIOC, GPIOC_DIR_M1);
        break;
    }
  }

  pwmEnableChannel(&PWM_MOTORS, pwm_channel, PWM_PERCENTAGE_TO_WIDTH(&PWM_MOTORS, width)); 
}
