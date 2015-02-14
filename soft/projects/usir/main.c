#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <avr/io.h>

static WORKING_AREA(waHeartbeatThread, 256);

msg_t heartbeatThread(void* arg) 
{
  (void)arg;

  while (1)
  {
    palSetPad(GPIOD, GPIOD_LED);
    chThdSleepMilliseconds(500);

    palClearPad(GPIOD, GPIOD_LED);
    chThdSleepMilliseconds(500);
  }

  return 0;
}


int main(void)
{
  //RTOS initialization
  halInit();
  chSysInit();

  chThdCreateStatic(waHeartbeatThread, sizeof(waHeartbeatThread), NORMALPRIO, heartbeatThread, NULL);

  while(1)
  {
    chThdSleepMilliseconds(100);
  }

  return 0;
}
