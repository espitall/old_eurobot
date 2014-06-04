#include <avr/io.h>
#include <FreeRTOS/module.h> 
#include <xmega/clock/clock.h> 
#include <com/com.h>
#include "config.h"

void led_heartbeat(void * p)
{
  (void)p;

  while(1) 
  {
    PORTQ.OUTTGL = (1 << 3);
    vTaskDelay(500);
  }
}

int main(void)
{
  clock_init();

  //initialize led
  PORTQ.DIRSET = (1 << 1);
  PORTQ.DIRSET = (1 << 2);
  PORTQ.DIRSET = (1 << 3);
  PORTQ.OUTCLR = (1 << 1);
  PORTQ.OUTCLR = (1 << 2);
  PORTQ.OUTCLR = (1 << 3);


  xTaskCreate(led_heartbeat, "heartbeat", 100, 0, SCHED_HEARTBEAT_PRIORITY, 0);
  com_init(SCHED_COM_PRIORITY);

  vTaskStartScheduler();

  return -1;
}
