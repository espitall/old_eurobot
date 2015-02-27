#include <hal.h>
#include "usir.h"

#if defined(USIR_MASTER)

static uint16_t usirRead16(uint8_t addr)
{
  addr &= 0x7F;

  uint8_t buffer[2];

  spiAcquireBus(&SPID5);
  boardSetCS(SPI_CS_EXT13);

  buffer[0] = addr;
  spiSend(&SPID5, 1, buffer);

  buffer[0] = 0xff;
  buffer[1] = 0xff;
  spiReceive(&SPID5, 2, buffer);

  boardSetCS(SPI_CS_NONE);
  spiReleaseBus(&SPID5);

  uint16_t data = (buffer[0] & 0xff) | ((buffer[1] & 0xff) << 8);

  return data;
}

void usirInit(void)
{
}

uint16_t usirGetDistancemm(int channel)
{
  return usirRead16(channel);
}

#endif

