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

  com_print(COM_INFO, "Boot register: 0x%02X", RST.STATUS);
  RST.STATUS = 0;

  while(1) 
  {
    PORTQ.OUTTGL = (1 << 3);
    vTaskDelay(2500);
    asserv_set_dist_set_point(30000);

    PORTQ.OUTTGL = (1 << 3);
    vTaskDelay(3000);
    asserv_set_dist_set_point(-30000);
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

void com_write_hook(void)
{
  static int32_t tickCount = 0;
  tickCount += (uint16_t)(xTaskGetTickCount() - ((uint16_t)tickCount));
  //com_print(COM_DEBUG, "speed l:%ld r:%ld", position_get_left_speed(), position_get_right_speed());

  //send asserv debug data
  uint8_t * buf = com_request_write_buffer(COM_BROADCAST, COM_PAYLOAD_ASSERV);
  buf[0] = COM_PAYLOAD_ASSERV_DEBUG_STREAM;
  com_payload_asserv_debug_stream_t * stream = (com_payload_asserv_debug_stream_t *) (buf + 1); 

  stream->timestamp = tickCount;
  stream->dist_set_point = asserv_get_dist_set_point();
  stream->dist_feedback = position_get_dist();
  stream->dist_p = asserv_get_dist_p();
  stream->dist_d = asserv_get_dist_d();
  stream->dist_output = asserv_get_dist_output();

  stream->angu_set_point = asserv_get_angu_set_point();
  stream->angu_feedback = position_get_angu();
  stream->angu_d = asserv_get_angu_d();
  stream->angu_output = asserv_get_angu_output();
  stream->angu_output = asserv_get_angu_output();

  com_release_write_buffer(sizeof(com_payload_asserv_debug_stream_t) + 1);
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
