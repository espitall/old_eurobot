#include <ch.h>
#include <hal.h>
#include "stepper.h"

static WORKING_AREA(waStepperThread, 256);
static uint16_t speed;
static int32_t position[4];
static int32_t target[4];

msg_t stepperThread(void* arg) 
{
  (void)arg;

  while (1)
  {
    uint8_t set = 0;
    int j;
    for(j = 0; j < 4; j += 1)
    {
      if(position[j] < target[j])
      {
        set |= (3 << 2 * j);
        position[j] += 1;
      }
      else if(position[j] > target[j])
      {
        set |= (2 << 2 * j);
        position[j] -= 1;
      }
    }

    PORTC.OUT = set;
    volatile int i;
    for(i= 0; i < 1000; i += 1);

    PORTC.OUTCLR = (1 << 5) | (1 << 7) | (1 << 3) | (1 << 1);
    chThdSleepMilliseconds(speed);
  }

  return 0;
}

void stepperInit(void)
{
  palSetPad(GPIOB, GPIOB_MS1);
  palClearPad(GPIOB, GPIOB_MS2);
  palClearPad(GPIOB, GPIOB_MS3);

  palSetPad(GPIOA, GPIOA_RESET);
  palSetPad(GPIOA, GPIOA_SLEEP);
  palClearPad(GPIOA, GPIOA_ENABLE);

  speed = 1;
  target[0] = 0;
  target[1] = 0;
  target[2] = 0;
  target[3] = 0;
  position[0] = 0;
  position[1] = 0;
  position[2] = 0;
  position[3] = 0;

  chThdCreateStatic(waStepperThread, sizeof(waStepperThread), IDLEPRIO + 1, stepperThread, NULL);
}

void stepperSetTarget(int id, int32_t pos)
{
  target[id] = pos;
}
