#include <ch.h>
#include <hal.h>
#include  "max7317.h"

void max7317Init(void)
{
  uint8_t tx[2];

  spiAcquireBus(&SPID5);

  boardSetCS(SPI_CS_IOE0);
  tx[0] = 0x0A;
  tx[1] = 0x01;
  spiSend(&SPID5, 2, tx);
  boardSetCS(SPI_CS_NONE);

  spiReleaseBus(&SPID5);
}

uint16_t max7317Read(void)
{
  uint16_t value;

  uint8_t tx[2];
  uint8_t rx[2];

  spiAcquireBus(&SPID5);

  boardSetCS(SPI_CS_IOE0);
  tx[0] = 0x8E;
  tx[1] = 0x00;
  spiExchange (&SPID5, 2, tx, rx);
  boardSetCS(SPI_CS_NONE);

  boardSetCS(SPI_CS_IOE0);
  tx[0] = 0x8F;
  tx[1] = 0x00;
  spiExchange (&SPID5, 2, tx, rx);
  boardSetCS(SPI_CS_NONE);
  value = rx[1];

  boardSetCS(SPI_CS_IOE0);
  tx[0] = 0x8F;
  tx[1] = 0x00;
  spiExchange (&SPID5, 2, tx, rx);
  boardSetCS(SPI_CS_NONE);
  value |= rx[1] << 8;

  spiReleaseBus(&SPID5);

  return value;
}
