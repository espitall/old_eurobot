#include <ch.h>
#include <hal.h>
#include "dc_motors.h"
#include "config.h"



static PWMConfig pwmcfg1 = {
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

static PWMConfig pwmcfg2 = {
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
   pwmStart(&PWMD1, &pwmcfg1);
   pwmStart(&PWMD2, &pwmcfg2);

   palSetPad(GPIOC, GPIOC_RST_MX);

   dcmSetWidth(0, 0);
   dcmSetWidth(1, 0);
}

void dcmSetWidth(unsigned int channel, int width)
{
  palSetPad(GPIOC, GPIOC_RST_MX);
  switch(channel)
  {
    case 0:
#if DC_MOTORS_TYPE == DC_MOTORS_TYPE_LMD18200T
      if(width < 0)
      {
        width = -width;
        width = 10000 - width;
        palSetPad(GPIOC, GPIOC_DIR_M0);
      }
      else
      {
        width = 10000 - width;
        palClearPad(GPIOC, GPIOC_DIR_M0);
      }
#else
      if(width < 0)
      {
        width = -width;
        width = 10000 - width;
        palSetPad(GPIOC, GPIOC_DIR_M0);
      }
      else
      {
        palClearPad(GPIOC, GPIOC_DIR_M0);
      }
#endif
      pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, width)); 
      break;

    case 1:
#if DC_MOTORS_TYPE == DC_MOTORS_TYPE_LMD18200T
      if(width < 0)
      {
        width = -width;
        width = 10000 - width;
        palClearPad(GPIOC, GPIOC_DIR_M1);
      }
      else
      {
        width = 10000 - width;
        palSetPad(GPIOC, GPIOC_DIR_M1);
      }
      pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, width)); 
#else
      if(width < 0)
      {
        width = -width;
        palClearPad(GPIOC, GPIOC_DIR_M1);
      }
      else
      {
        width = 10000 - width;
        palSetPad(GPIOC, GPIOC_DIR_M1);
      }
      pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, width)); 
#endif
      break;

    default:
      return;
  }
}
