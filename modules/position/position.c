#include <FreeRTOS/module.h> 
#include "encoder_magnetic/encoder_magnetic.h"
#include "position.h"
#include "position_config.h"

/* Remove for now
static Peripheral_Descriptor_t spi;
*/

static int32_t pos_left_tick;
static int32_t pos_right_tick;
static int32_t pos_dist;
static int32_t pos_angu;

static int32_t spd_left_tick;
static int32_t spd_right_tick;
static int32_t spd_dist;
static int32_t spd_angu;

void position_init(void)
{
  //spi = FreeRTOS_open((const int8_t *)POSITION_SPI_PERIPHERAL, 0);
  //encoder_magnetic_init(&spi);
}

void position_update(void)
{
  encoder_magnetic_manage();

  int32_t new_pos_left_tick = - encoder_magnetic_get_value(1);
  int32_t new_pos_right_tick = encoder_magnetic_get_value(0);

  spd_left_tick = new_pos_left_tick - pos_left_tick;
  spd_right_tick = new_pos_right_tick - pos_right_tick;
  pos_left_tick = new_pos_left_tick;
  pos_right_tick = new_pos_right_tick;

  pos_dist = pos_right_tick + pos_left_tick;
  pos_angu = pos_right_tick - pos_left_tick;

  spd_dist = spd_right_tick + spd_left_tick;
  spd_angu = spd_right_tick - spd_left_tick;
}

int32_t position_get_right_speed(void)
{
  return spd_right_tick;
}

int32_t position_get_left_speed(void)
{
  return spd_left_tick;
}


int32_t position_get_right(void)
{
  return pos_right_tick;
}

int32_t position_get_left(void)
{
  return pos_left_tick;
}

int32_t position_get_dist(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = pos_dist;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t position_get_angu(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = pos_angu;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t position_get_dist_speed(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = spd_dist;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t position_get_angu_speed(void)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = spd_angu;
  }
  taskEXIT_CRITICAL();

	return value;
}

int32_t position_from_cm(double cm)
{
  return POSITION_TICK_PER_CM * cm;
}
