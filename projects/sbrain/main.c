#include <avr/io.h>
#include <FreeRTOS/module.h> 
#include <xmega/clock/clock.h> 
#include <position/position.h> 
#include <asserv/asserv.h> 
#include <com/com.h>
#include "motors.h"
#include "config.h"

void led_heartbeat(void * p)
{
  (void)p;

  //handle reset
  com_print_lvl_t lvl = COM_ERROR;
  if((RST.STATUS & RST_PDIRF_bm) || (RST.STATUS == 0))
  {
    lvl = COM_INFO;
  }
  com_print(lvl, "Boot: %s%s%s%s%s%s%s%s", 
            (RST.STATUS & RST_SDRF_bm) ? "spike " : "",
            (RST.STATUS & RST_SRF_bm) ? "softreg " : "",
            (RST.STATUS & RST_PDIRF_bm) ? "pdi " : "",
            (RST.STATUS & RST_WDRF_bm) ? "watchdog " : "",
            (RST.STATUS & RST_BORF_bm) ? "brown-out " : "",
            (RST.STATUS & RST_EXTRF_bm) ? "pin " : "",
            (RST.STATUS & RST_PORF_bm) ? "power-on " : "",
            ((RST.STATUS & 0xff) == 0) ? "soft" : ""
            );
  RST.STATUS = 0xff;

  vTaskDelay(3000);
  while(1) 
  {
    PORTQ.OUTSET = (1 << 3);
    PORTQ.OUTCLR = (1 << 2);
    asserv_set_dist_set_point(position_from_cm(33.5 * 3));
    vTaskDelay(10000);


    PORTQ.OUTSET = (1 << 2);
    PORTQ.OUTCLR = (1 << 3);
    asserv_set_dist_set_point(position_from_cm(-33.5));
    vTaskDelay(10000);
  }
}

void asserv(void *p)
{
  TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t asserv_period = 20;

  while(1)
  {
    vTaskDelayUntil( &last_wake_time, asserv_period);

    position_update();
    asserv_update();
    motors_set_consign(asserv_get_left_mot_set_point(), asserv_get_right_mot_set_point());
  }
}

uint32_t com_get_ith_hook(com_packet_header_t * header)
{
  //always route packet to interface 0
  (void)header;
  return (1 << 0);
}

void com_write_hook(uint32_t tickCount)
{
  //com_print(COM_DEBUG, "speed l:%ld r:%ld", position_get_left(), position_get_right());

  //send asserv debug data
  asserv_com_write_handler(tickCount);
}

unsigned int com_read_hook(com_packet_header_t * header, uint8_t * buf)
{
  (void)header;
  (void)buf;
  
  if(asserv_com_read_handler(header, buf))
  {
    return 1;
  }

  return 0;
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
  asserv_init();
  motors_init();
  com_init(SCHED_COM_RECV_PRIORITY, SCHED_COM_SEND_PRIORITY);
  xTaskCreate(led_heartbeat, "heartbeat", 200, 0, SCHED_HEARTBEAT_PRIORITY, 0);
  xTaskCreate(asserv, "asserv", 512, 0, SCHED_ASSERV_PRIORITY, 0);


  //start scheduler
  vTaskStartScheduler();

  return -1;
}
