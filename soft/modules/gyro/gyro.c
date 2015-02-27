#include <hal.h>
#include <lcd.h>
#include "gyro.h"

void gyroRead(uint16_t addr, uint16_t *data)
{
  (void)data;
  char buftx[4];
  char bufrx[4];
  int i,j;

  for(i = 0; i < 4; i += 1)
  {
    buftx[i] = 0;
    bufrx[i] = 0;
  }

  buftx[0] = 0x80 | ((addr >> 7) & 0x03);
  buftx[1] = (addr << 1) & 0xFE;
  buftx[2] = 0;
  buftx[3] = 0;

  int parity = 0;
  for(i = 0; i < 4; i += 1)
  {
    for(j = 0; j < 8; j += 1)
    {
      if(buftx[i] & (1 << j))
      {
        parity = !parity;
      }
    }
  }

  buftx[3] |= !parity;

  spiAcquireBus(&SPID4);
  palClearPad(GPIOE, GPIOE_SPI4_GYRO_CS);
  spiSend(&SPID4, 4, buftx);
  palSetPad(GPIOE, GPIOE_SPI4_GYRO_CS);
  spiReleaseBus(&SPID4);

  chThdSleepMilliseconds(1);

  spiAcquireBus(&SPID4);
  palClearPad(GPIOE, GPIOE_SPI4_GYRO_CS);
  spiExchange(&SPID5, 4, buftx, bufrx);
  palSetPad(GPIOE, GPIOE_SPI4_GYRO_CS);
  spiReleaseBus(&SPID4);

  
}

void gyroWrite(uint16_t addr, uint16_t *data)
{
  char buftx[4];
  char bufrx[4];
  (void)bufrx;
  int i,j;

  for(i = 0; i < 4; i += 1)
  {
    buftx[i] = 0;
    bufrx[i] = 0;
  }

  buftx[0] = 0x80 | ((addr >> 7) & 0x03);
  buftx[1] = (addr << 1) & 0xFE;
  buftx[2] = 0;
  buftx[3] = 0;

  int parity = 0;
  for(i = 0; i < 4; i += 1)
  {
    for(j = 0; j < 8; j += 1)
    {
      if(buftx[i] & (1 << j))
      {
        parity = !parity;
      }
    }
  }

  buftx[3] |= !parity;
  (void)data;
} 

void gyroInit(void)
{
  palSetPad(GPIOE, GPIOE_SPI4_GYRO_CS);

  lcdPrintln("Init: gyro");
}
