#include <string.h>
#include "encoder_magnetic.h"
#include "encoder_magnetic_config.h"

static Peripheral_Descriptor_t * spi;

static int32_t values[ENCODER_MAGNETIC_NUMBER];
static int16_t previous[ENCODER_MAGNETIC_NUMBER];

static portGPIO_TYPE * gpio_port[ENCODER_MAGNETIC_NUMBER] = ENCODER_MAGNETIC_SS_PORTS;
static uint8_t gpio_pin[ENCODER_MAGNETIC_NUMBER] = ENCODER_MAGNETIC_SS_BITS;


void encoder_magnetic_init(Peripheral_Descriptor_t * s)
{
  spi = s;

  int i;
	for (i = 0; i < ENCODER_MAGNETIC_NUMBER; i++)
  {
    portGPIO_DIRSET((*gpio_port[i]), (1 << gpio_pin[i]));
    portGPIO_OUTSET((*gpio_port[i]), (1 << gpio_pin[i]));
  }

	//encoder_magnetic_manage();
	memset(values, 0, sizeof(values));
}

void encoder_magnetic_manage(void)
{
	union {
		struct {
			uint8_t u8_lsb;
			uint8_t u8_msb;
		} s;
		int16_t s16;
	} enc;
	uint8_t i;
	int16_t diff;

	for (i = 0; i < ENCODER_MAGNETIC_NUMBER; i++)
  {
    portGPIO_OUTCLR((*gpio_port[i]), (1 << gpio_pin[i]));
    uint8_t raw[2];
    FreeRTOS_read(*spi, raw, 2);
		enc.s.u8_msb = raw[0] & 0xff;
		enc.s.u8_lsb = raw[1] & 0xff;
    enc.s16 <<= 1;
    portGPIO_OUTSET((*gpio_port[i]), (1 << gpio_pin[i]));

		diff = enc.s16 - previous[i];
		previous[i] = enc.s16;

    taskENTER_CRITICAL();
    {
      values[i] += diff;
    }
    taskEXIT_CRITICAL();
	}
}

int32_t encoder_magnetic_get_value(int id)
{
	int32_t value;

  taskENTER_CRITICAL();
  {
    value = values[id];
  }
  taskEXIT_CRITICAL();

	return value;
}
