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
  lcdPrintln(LCD_INFO, "USIR: initialisation");
  int i;
  uint16_t board_id = 0x42;

  for(i = 0; i < 50; i += 1)
  {
    board_id = usirRead16(USIR_BOARD_ID);
    if(board_id == USIR_ID)
    {
      lcdPrintln(LCD_INFO, "USIR: OK");
      return;
    }
    chThdSleepMilliseconds(100);
  }
  lcdPrintln(LCD_ERROR, "USIR: HS (0x%04X, 0x%04X)", board_id, USIR_ID);
}

uint16_t usirGetDistancemm(int channel)
{
  return usirRead16(channel);
}

#endif

