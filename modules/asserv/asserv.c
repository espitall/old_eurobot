#include <FreeRTOS/module.h> 
#include <position/position.h>
#include "asserv.h"

static int32_t left_mot_set_point;
static int32_t right_mot_set_point;

static int32_t angu_set_point;
static int32_t dist_set_point;

int32_t dist_pid_shift_out;
int32_t dist_pid_kp;
int32_t dist_pid_kd;
int32_t dist_pid_output;
int32_t dist_pid_e;
int32_t dist_pid_p;
int32_t dist_pid_d;

int32_t angu_pid_shift_out;
int32_t angu_pid_kp;
int32_t angu_pid_kd;
int32_t angu_pid_output;
int32_t angu_pid_e;
int32_t angu_pid_p;
int32_t angu_pid_d;


void asserv_init(void)
{
  angu_set_point = 0;
  dist_set_point = 0;

  left_mot_set_point = 0;
  right_mot_set_point = 0;

  dist_pid_shift_out = 14;
  dist_pid_kp = 210;
  dist_pid_kd = 30;
  dist_pid_e = 0;
  dist_pid_p = 0;
  dist_pid_d = 0;
  dist_pid_output = 0;
  
  angu_pid_shift_out = 14;
  angu_pid_kp = 150;
  angu_pid_kd = 0;
  angu_pid_e = 0;
  angu_pid_p = 0;
  angu_pid_d = 0;
  angu_pid_output = 0;
}

void asserv_update(void)
{
  int32_t pid_d = dist_set_point - position_get_dist();
  int32_t pid_a = angu_set_point - position_get_angu();

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
