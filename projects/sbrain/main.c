#include <avr/io.h>
#include <FreeRTOS/module.h> 
#include <xmega/clock/clock.h> 
#include <position/position.h> 
#include <com/com.h>
#include "motors.h"
#include "config.h"

void led_heartbeat(void * p)
{
  (void)p;

  while(1) 
  {
    PORTQ.OUTTGL = (1 << 3);
    vTaskDelay(500);
    motors_set_consign(0, 300);
  }
}

void asserv(void *p)
{
  while(1)
  {
    position_update();
    vTaskDelay(10);
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
  com_print(COM_DEBUG, "speed %ld", position_get_right_speed());
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
  position_init();
  motors_init();
  com_init(SCHED_COM_RECV_PRIORITY, SCHED_COM_SEND_PRIORITY);
  xTaskCreate(led_heartbeat, "heartbeat", 200, 0, SCHED_HEARTBEAT_PRIORITY, 0);
  xTaskCreate(asserv, "asserv", 200, 0, SCHED_ASSERV_PRIORITY, 0);


  //start scheduler
  vTaskStartScheduler();

  return -1;
}
