#include <hal.h>
#include "usir.h"

#if defined(USIR_MASTER)
#include <lcd.h>

static uint16_t usirRead16(uint8_t addr)
{
  addr &= 0x7F;

  uint8_t buffer[2];

  spiAcquireBus(&SPID5);
  boardSetCS(SPI_CS_EXT13);
  buffer[0] = 0x80 | (addr & 0x7F);
  spiSend(&SPID5, 1, buffer);
  boardSetCS(SPI_CS_NONE);
  spiReleaseBus(&SPID5);

  spiAcquireBus(&SPID5);
  boardSetCS(SPI_CS_EXT13);

  buffer[0] = 0xff;
  buffer[1] = 0xff;
  spiExchange(&SPID5, 2, buffer, buffer);
  boardSetCS(SPI_CS_NONE);
  spiReleaseBus(&SPID5);

  uint16_t data = (buffer[0] & 0xff) | ((buffer[1] & 0xff) << 8);

  return data;
}

void usirInit(void)
{
  //int i;
  //for(i = 0; i < 5; i += 1)
  //{
  //  uint16_t board_id = usirRead16(USIR_BOARD_ID);
  //  if(board_id == USIR_ID)
  //  {
  //    lcdPrintln(LCD_INFO, "Carte USIR ok");
  //    return;
  //  }
  //  chThdSleepMilliseconds(1000);
  //}
  //lcdPrintln(LCD_ERROR, "Carte USIR hors service");
}

uint16_t usirGetDistancemm(int channel)
{
  return usirRead16(channel);
}

#endif

