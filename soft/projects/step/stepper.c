#include <ch.h>
#include <hal.h>
#include "stepper.h"

static MUTEX_DECL(mutex);
static WORKING_AREA(waStepperThread, 256);
static volatile uint16_t speed;
static volatile int32_t position[4];
static volatile int32_t target[4];

msg_t stepperThread(void* arg) 
{
  (void)arg;

  while (1)
  {
    chMtxLock(&mutex);
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
    chMtxUnlock();

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

  chMtxInit(&mutex);

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
  chMtxLock(&mutex);
  target[id] = pos;
  chMtxUnlock();
}

void stepperSetRel(int id, int32_t pos)
{
  chMtxLock(&mutex);
  target[id] = pos + position[id];;
  chMtxUnlock();
}

void stepperSetPosition(int id, int32_t pos)
{
  chMtxLock(&mutex);
  position[id] = pos;
  target[id] = pos;
  chMtxUnlock();
}

void stepperWait(int id)
{
  int32_t t;
  int32_t p;
  do
  {
    chMtxLock(&mutex);
    p = position[id];
    t = target[id];
    chMtxUnlock();
    if(p != t)
    {
      chThdSleepMilliseconds(50);
    }
  }while(p != t);
}

void stepperDisable(void)
{
  palSetPad(GPIOA, GPIOA_ENABLE);
  palClearPad(GPIOA, GPIOA_RESET);
  palClearPad(GPIOA, GPIOA_SLEEP);
}
