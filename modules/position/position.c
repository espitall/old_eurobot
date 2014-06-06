#include <FreeRTOS/module.h> 
#include "encoder_magnetic/encoder_magnetic.h"
#include "position.h"
#include "position_config.h"


static Peripheral_Descriptor_t spi;

void position_init(void)
{
  spi = FreeRTOS_open((const int8_t *)POSITION_SPI_PERIPHERAL, 0);
  encoder_magnetic_init(&spi);
}

void position_update(void)
{
  encoder_magnetic_manage();
}

int32_t position_get_right_speed(void)
{
  return encoder_magnetic_get_value(0);
}
