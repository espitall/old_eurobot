#include <FreeRTOS/module.h> 
#include "position.h"
#include "position_config.h"


static Peripheral_Descriptor_t spi;

void position_init(void)
{
  spi = FreeRTOS_open((const int8_t *)POSITION_SPI_PERIPHERAL, 0);
  if(spi == NULL)
  {
    PORTQ.OUTSET = (1 << 2);
  }
}

void position_update(void)
{

}

int32_t position_get_right_speed(void)
{
  return 42;
}
