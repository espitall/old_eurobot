#include <hal.h>
#include <lcd.h>
#include <stdint.h>
#include "gyro.h"
#include "../lcd/lcd.h"

void gyroRead(uint16_t addr, uint16_t *data)
{
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
  spiExchange(&SPID4, 4, buftx, bufrx);
  palSetPad(GPIOE, GPIOE_SPI4_GYRO_CS);
  spiReleaseBus(&SPID4);

  *data = ((bufrx [1] & 0b00011111) << 11) | (bufrx [2] << 3) | ((bufrx [3] & 0b11100000) >> 5);
}

void gyroGetRawValue(uint16_t *data)
{
  char buftx[4];
  char bufrx[4];
  int i,j;

  for(i = 0; i < 4; i += 1)
  {
    buftx[i] = 0;
    bufrx[i] = 0;
  }

  buftx[0] = 0b00100000;
  buftx[1] = 0b00000000;
  buftx[2] = 0b00000000;
  buftx[3] = 0b00000000;

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
  spiExchange(&SPID4, 4, buftx, bufrx);
  palSetPad(GPIOE, GPIOE_SPI4_GYRO_CS);
  spiReleaseBus(&SPID4);

  *data = ((bufrx [0] & 0b00000011) << 14) | (bufrx [1] << 6) | ((bufrx [2] & 0b11111100) >> 2);
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

  chThdSleepMilliseconds(1);

  lcdPrintln("Init: gyro");

  int32_t somme = 0;
  int i;
  for (i = 0; i < 1000; i++)
  {
    chThdSleepMilliseconds(1);
    uint16_t buff;
    gyroGetRawValue(&buff);
    int16_t val = (int) buff;
    somme += val;
  }

  int32_t delta = somme / 1000;
  lcdPrintln("Delta : %ld", delta);
  /*
  somme = 0;
  int cpt = 0;
  while (true)
  {
    cpt++;
    chThdSleepMilliseconds(1);
    uint16_t buff;
    gyroGetRawValue(&buff);
    int16_t val = (int) buff - delta;
    somme += val;
    if (cpt%1000 == 0)
      lcdPrintln("%ld", somme/80/1000);
  }
  */
}
