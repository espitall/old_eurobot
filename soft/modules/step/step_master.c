#include <hal.h>
#include "step.h"

#if defined(STEP_MASTER)
#include <lcd.h>

static uint16_t stepRead16(uint8_t addr)
{
  addr &= 0x7F;

  uint8_t txbuffer[2];
  uint8_t rxbuffer[2];

  spiAcquireBus(&SPID5);
  boardSetCS(SPI_CS_EXT12);
  txbuffer[0] = 0x80 | (addr & 0x7F);
  spiSend(&SPID5, 1, txbuffer);
  boardSetCS(SPI_CS_NONE);
  spiReleaseBus(&SPID5);

  spiAcquireBus(&SPID5);
  boardSetCS(SPI_CS_EXT12);

  txbuffer[0] = 0x80;
  txbuffer[1] = 0x80;
  spiExchange(&SPID5, 2, txbuffer, rxbuffer);
  boardSetCS(SPI_CS_NONE);
  spiReleaseBus(&SPID5);

  uint16_t data = (rxbuffer[0] & 0xff) | ((rxbuffer[1] & 0xff) << 8);

  return data;
}

static void stepWrite16(uint8_t addr, uint16_t value)
{
  addr &= 0x7F;

  uint8_t buffer[3];

  spiAcquireBus(&SPID5);
  boardSetCS(SPI_CS_EXT12);
  buffer[0] = addr & 0x7F;
  buffer[1] = value & 0xff;
  buffer[2] = (value >> 8) & 0xff;
  spiSend(&SPID5, 3, buffer);
  boardSetCS(SPI_CS_NONE);
  spiReleaseBus(&SPID5);
}

void stepInit(void)
{
  int i;
  uint16_t board_id = 0x42;

  lcdPrintln(LCD_INFO, "Step: initialisation");
  for(i = 0; i < 50; i += 1)
  {
    board_id = stepRead16(STEP_BOARD_ID);
    if(board_id == STEP_ID)
    {
      lcdPrintln(LCD_INFO, "Step: OK");

      return;
    }
    chThdSleepMilliseconds(100);
  }
  lcdPrintln(LCD_ERROR, "Step: HS (0x%04X, 0x%04X)", board_id, STEP_ID);
}

void stepWait(void)
{
  while(true)
  {
    uint16_t sta = stepRead16(STEP_COMMAND);
    if(!(sta & STEP_STATUS_BUSY))
    {
      return;
    }
    chThdSleepMilliseconds(500);
  }
}

void stepAction(step_action_t action)
{
  stepWrite16(STEP_COMMAND, action);
}

#endif


