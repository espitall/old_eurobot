#include <FreeRTOS/module.h> 
#include <position/position.h>
#include "asserv.h"
#include "payload.h"

static int32_t left_mot_set_point;
static int32_t right_mot_set_point;


static int32_t dist_set_point;
static int32_t dist_speed_set_point;
static int32_t dist_pid_set_point;
static int32_t dist_pid_shift_out;
static int32_t dist_pid_kp;
static int32_t dist_pid_kd;
static int32_t dist_pid_output;
static int32_t dist_pid_e;
static int32_t dist_pid_p;
static int32_t dist_pid_d;
static int32_t dist_vmax;
static int32_t dist_amax;

static int32_t angu_set_point;
static int32_t angu_speed_set_point;
static int32_t angu_pid_set_point;
static int32_t angu_pid_shift_out;
static int32_t angu_pid_kp;
static int32_t angu_pid_kd;
static int32_t angu_pid_output;
static int32_t angu_pid_e;
static int32_t angu_pid_p;
static int32_t angu_pid_d;
static int32_t angu_vmax;
static int32_t angu_amax;


void asserv_init(void)
{
  angu_set_point = 0;
  dist_set_point = 0;

  left_mot_set_point = 0;
  right_mot_set_point = 0;

  dist_pid_shift_out = 14;
  dist_pid_kp = 400;
  dist_pid_kd = 1000;
  dist_amax = 150;
  dist_vmax = 5000;
  dist_pid_e = 0;
  dist_pid_p = 0;
  dist_pid_d = 0;
  dist_pid_output = 0;
  dist_speed_set_point = 0;
  
  angu_pid_shift_out = 14;
  angu_pid_kp = 200;
  angu_pid_kd = 500;
  angu_amax = 10;
  angu_vmax = 5;
  angu_pid_e = 0;
  angu_pid_p = 0;
  angu_pid_d = 0;
  angu_pid_output = 0;
}

void asserv_update(void)
{
  //speed limitation
  if(dist_speed_set_point > 0)
  {
    if((dist_speed_set_point * dist_speed_set_point) / (2 * dist_amax) + dist_pid_set_point < dist_set_point)
    {
      if(dist_speed_set_point < dist_vmax)
      {
        dist_speed_set_point += dist_amax;
      }
    }
    else
    {
      if(dist_speed_set_point > -dist_vmax)
      {
        dist_speed_set_point -= dist_amax;
      }
    }
  }
  else
  {
    if(-(dist_speed_set_point * dist_speed_set_point) / (2 * dist_amax) + dist_pid_set_point < dist_set_point)
    {
      if(dist_speed_set_point < dist_vmax)
      {
        dist_speed_set_point += dist_amax;
      }
    }
    else
    {
      if(dist_speed_set_point > -dist_vmax)
      {
        dist_speed_set_point -= dist_amax;
      }
    }
  }
  dist_pid_set_point += dist_speed_set_point;

  angu_speed_set_point = 0;
  angu_pid_set_point = angu_set_point;


  //PID
  int32_t pid_d = dist_pid_set_point - position_get_dist();
  int32_t pid_a = angu_pid_set_point - position_get_angu();

  dist_pid_p = dist_pid_kp * pid_d;
  dist_pid_d = dist_pid_kd * (pid_d - dist_pid_e);
  dist_pid_e = pid_d;
  dist_pid_output = dist_pid_p + dist_pid_d;

  dist_pid_p >>= dist_pid_shift_out;
  dist_pid_d >>= dist_pid_shift_out;
  dist_pid_output >>= dist_pid_shift_out;


  angu_pid_p = angu_pid_kp * pid_a;
  angu_pid_d = angu_pid_kd * (pid_a - angu_pid_e);
  angu_pid_e = pid_a;
  angu_pid_output = angu_pid_p + angu_pid_d;

  angu_pid_p >>= angu_pid_shift_out;
  angu_pid_d >>= angu_pid_shift_out;
  angu_pid_output >>= angu_pid_shift_out;


  left_mot_set_point = dist_pid_output - angu_pid_output;
  right_mot_set_point = dist_pid_output + angu_pid_output;
}

int32_t asserv_get_left_mot_set_point(void)
{
  return left_mot_set_point;
}

int32_t asserv_get_right_mot_set_point(void)
{
  return right_mot_set_point;
}

void asserv_set_dist_set_point(int32_t sp)
{
  taskENTER_CRITICAL();
  {
    dist_set_point = sp;
  }
  taskEXIT_CRITICAL();
}

int32_t asserv_get_dist_set_point(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = dist_set_point;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_dist_output(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = dist_pid_output;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_dist_p(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = dist_pid_p;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_dist_d(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = dist_pid_d;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_dist_speed_set_point(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = dist_speed_set_point;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_dist_pid_set_point(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = dist_pid_set_point;
  }
  taskEXIT_CRITICAL();

	return value;
}

void asserv_set_angu_set_point(int32_t sp)
{
  taskENTER_CRITICAL();
  {
    angu_set_point = sp;
  }
  taskEXIT_CRITICAL();
}

int32_t asserv_get_angu_set_point(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = angu_set_point;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_angu_output(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = angu_pid_output;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_angu_d(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = angu_pid_d;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_angu_speed_set_point(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = angu_speed_set_point;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_angu_pid_set_point(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = angu_pid_set_point;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t asserv_get_angu_p(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = angu_pid_p;
  }
  taskEXIT_CRITICAL();

	return value;
}

/* Remove for now
void asserv_com_write_handler(uint32_t tickCount)
{
  uint8_t * buf = com_request_write_buffer(COM_BROADCAST, COM_PAYLOAD_ASSERV);
  buf[0] = ASSERV_PAYLOAD_DEBUG_STREAM;
  asserv_payload_debug_stream_t * stream = (asserv_payload_debug_stream_t *) (buf + 1); 

  stream->timestamp = tickCount;

  stream->dist_set_point = asserv_get_dist_set_point();
  stream->dist_pid_set_point = asserv_get_dist_pid_set_point();
  stream->dist_speed_set_point = asserv_get_dist_speed_set_point();
  stream->dist_speed_feedback = position_get_dist_speed();
  stream->dist_feedback = position_get_dist();
  stream->dist_p = asserv_get_dist_p();
  stream->dist_d = asserv_get_dist_d();
  stream->dist_output = asserv_get_dist_output();

  stream->angu_set_point = asserv_get_angu_set_point();
  stream->angu_pid_set_point = asserv_get_angu_pid_set_point();
  stream->angu_speed_set_point = asserv_get_angu_speed_set_point();
  stream->angu_speed_feedback = position_get_angu_speed();
  stream->angu_feedback = position_get_angu();
  stream->angu_d = asserv_get_angu_d();
  stream->angu_output = asserv_get_angu_output();
  stream->angu_output = asserv_get_angu_output();

  com_release_write_buffer(sizeof(asserv_payload_debug_stream_t) + 1);
}

unsigned int asserv_com_read_handler(com_packet_header_t * header, uint8_t * buf)
{
  if(header->payload_type == COM_PAYLOAD_ASSERV)
  {
    switch(buf[0])
    {
      case ASSERV_PAYLOAD_READ_D_PARAMS:
        {
          uint8_t * wbuf = com_request_write_buffer(header->src, COM_PAYLOAD_ASSERV);
          wbuf[0] = ASSERV_PAYLOAD_READ_D_PARAMS;
          asserv_payload_params_t * st = (asserv_payload_params_t *) (wbuf + 1); 
          st->kp = dist_pid_kp;
          st->kd = dist_pid_kd;
          st->shift = dist_pid_shift_out;
          st->amax = dist_amax;
          st->vmax = dist_vmax;
          com_release_write_buffer(sizeof(asserv_payload_params_t) + 1);
        }
        break;

      case ASSERV_PAYLOAD_READ_A_PARAMS:
        {
          uint8_t * wbuf = com_request_write_buffer(header->src, COM_PAYLOAD_ASSERV);
          wbuf[0] = ASSERV_PAYLOAD_READ_A_PARAMS;
          asserv_payload_params_t * st = (asserv_payload_params_t *) (wbuf + 1); 
          st->kp = angu_pid_kp;
          st->kd = angu_pid_kd;
          st->shift = angu_pid_shift_out;
          st->amax = angu_amax;
          st->vmax = angu_vmax;
          com_release_write_buffer(sizeof(asserv_payload_params_t) + 1);
        }
        break;

      case ASSERV_PAYLOAD_WRITE_D_PARAMS:
        {
          asserv_payload_params_t * st = (asserv_payload_params_t *) (buf + 1); 
          dist_pid_kp = st->kp;
          dist_pid_kd = st->kd;
          dist_pid_shift_out = st->shift;
          dist_amax = st->amax;
          dist_vmax = st->vmax;
        }
        break;

      case ASSERV_PAYLOAD_WRITE_A_PARAMS:
        {
          asserv_payload_params_t * st = (asserv_payload_params_t *) (buf + 1); 
          angu_pid_kp = st->kp;
          angu_pid_kd = st->kd;
          angu_pid_shift_out = st->shift;
          angu_amax = st->amax;
          angu_vmax = st->vmax;
        }
        break;

      default:
        com_print(COM_ERROR, "Asserv payload id 0x%02X not implemented", buf[0]);
        break;
    }
    return 1;
  }
  return 0;
}
*/
