#include <FreeRTOS/module.h> 
#include <position/position.h>
#include "asserv.h"

static int32_t left_mot_set_point;
static int32_t right_mot_set_point;

static int32_t angu_set_point;
static int32_t dist_set_point;

int32_t dist_pid_shift_out;
int32_t dist_pid_kp;
int32_t dist_pid_output;

int32_t angu_pid_shift_out;
int32_t angu_pid_kp;
int32_t angu_pid_output;


void asserv_init(void)
{
  angu_set_point = 0;
  dist_set_point = 0;

  left_mot_set_point = 0;
  right_mot_set_point = 0;

  dist_pid_shift_out = 12;
  dist_pid_kp = 70;
  
  angu_pid_shift_out = 12;
  angu_pid_kp = 50;
}

void asserv_update(void)
{
  int32_t pid_d = dist_set_point - position_get_dist();
  int32_t pid_a = angu_set_point - position_get_angu();

  pid_d = dist_pid_kp * pid_d;
  pid_d >>= dist_pid_shift_out;
  dist_pid_output = pid_d;

  pid_a = angu_pid_kp * pid_a;
  pid_a >>= angu_pid_shift_out;
  angu_pid_output = pid_a;


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

void asserv_set_angu_set_point(int32_t sp)
{
  taskENTER_CRITICAL();
  {
    angu_set_point = sp;
  }
  taskEXIT_CRITICAL();
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
