#include <avr/io.h>
#include <FreeRTOS/module.h> 
#include <xmega/clock/clock.h> 
#include <com/com.h>
#include "motors.h"
#include "config.h"

void led_heartbeat(void * p)
{
  (void)p;

  int cnt = 0;
  while(1) 
  {
    PORTQ.OUTTGL = (1 << 3);
    com_print(COM_DEBUG, "heartbeat %d", cnt++);
    vTaskDelay(500);
  }
}

uint32_t com_get_ith_hook(com_packet_header_t * header)
{
  //always route packet to interface 0
  (void)header;
  return (1 << 0);
}

void com_write_hook(void)
{
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


  //initialize modules
  motors_init();
  com_init(SCHED_COM_RECV_PRIORITY, SCHED_COM_SEND_PRIORITY);
  xTaskCreate(led_heartbeat, "heartbeat", 200, 0, SCHED_HEARTBEAT_PRIORITY, 0);


  //start scheduler
  vTaskStartScheduler();

  return -1;
}
