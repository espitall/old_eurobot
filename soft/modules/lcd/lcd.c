#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <chprintf.h>

#include "lcd.h"
#include "ili9341.h"
#include "tm_fonts.h"
#include "config.h"

typedef struct {
  struct BaseSequentialStreamVMT * vmt;
  TM_FontDef_t * font;
  int startX;
  int startY;
  int colPosition;
  int rowPosition;
  int maxRow;
  int maxCol;
  int rowModulo;
} lcdPrintDriver_t;

static uint16_t frame_buffer[ILI9341_PIXEL];

struct BaseSequentialStreamVMT lcdPrintDriverVMT;
lcdPrintDriver_t topLine, console;

static WORKING_AREA(waLcdThread, 128);


static void lcdSetPixel(uint16_t x, uint16_t y, uint16_t color)
{
  if((x < ILI9341_HEIGHT) && (y < ILI9341_WIDTH))
  {
    frame_buffer[x * ILI9341_WIDTH + (ILI9341_WIDTH - y - 1)] = color;
  }
}

static uint16_t lcdGetPixel(uint16_t x, uint16_t y)
{
  if((x < ILI9341_HEIGHT) && (y < ILI9341_WIDTH))
  {
    return frame_buffer[x * ILI9341_WIDTH + (ILI9341_WIDTH - y - 1)];
  }
  return 0;
}

static msg_t lcdPutChar(void * instance, uint8_t c) 
{

  lcdPrintDriver_t * drv = (lcdPrintDriver_t *) instance;

  if(drv->colPosition >= drv->maxCol)
  {
    drv->colPosition = 0;
    drv->rowPosition += 1;
  }

  uint8_t need_reset_line = 0;
  while(drv->rowPosition >= drv->maxRow)
  {
    int x;
    int y;
    for(y = 0; y < drv->font->FontHeight * (drv->maxRow - 1); y += 1)
    {
      for(x = 0; x < drv->font->FontWidth * drv->maxCol; x += 1)
      {
        if(drv->rowModulo == 1)
        {
          lcdSetPixel(x + drv->startX, y + drv->startY, lcdGetPixel(x + drv->startX, y + drv->startY + drv->font->FontHeight));
        }
        else
        {
          lcdSetPixel(x + drv->startX, y + drv->startY, 0x00);
        }
      }
    }

    need_reset_line = 1;
    drv->rowPosition -= drv->rowModulo;
  }
  if(need_reset_line)
  {
    int x;
    int y;
    for(y = 0; y < drv->font->FontHeight; y += 1)
    {
      for(x = 0; x < drv->font->FontWidth * drv->maxCol; x += 1)
      {
        lcdSetPixel(x + drv->startX, y + drv->startY + drv->font->FontHeight * drv->rowPosition, 0x0000);
      }
    }
  }

  uint16_t xpos = drv->startX + drv->colPosition * drv->font->FontWidth;
  uint16_t ypos = drv->startY + drv->rowPosition * drv->font->FontHeight;

  if(c != '\n') 
  {
    uint16_t i,j,b;
    for (i = 0; i < drv->font->FontHeight; i++) 
    {
      b = drv->font->data[(c - 32) * drv->font->FontHeight + i];
      for (j = 0; j < drv->font->FontWidth; j++) 
      {
        if ((b << j) & 0x8000) 
        {
          lcdSetPixel(xpos + j, ypos + i, 0xFFFF);
        } 
        else
        {
          lcdSetPixel(xpos + j, ypos + i, 0x0000);
        }
      }
    }
    drv->colPosition += 1;
  }
  else
  {
    drv->rowPosition += 1;
    drv->colPosition = 0;
  }


  return 0;
}

static void lcdvPrintArea(lcdPrintDriver_t * drv, const char * fmt, va_list ap)
{
  chvprintf((BaseSequentialStream *) drv, fmt, ap);
}

static void lcdPrintArea(lcdPrintDriver_t * drv, const char * fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  lcdvPrintArea(drv, fmt, ap);

  va_end(ap);
}

static msg_t lcdThread(void *arg) {
  (void) arg;

  while (TRUE) {
    int32_t x_mm = 12;
    int32_t y_mm = 35000;
    int32_t arel_deg = 174;
    int32_t a_deg = 35174;
    uint8_t pos_cor = 99;
    uint8_t time = 119;
    uint8_t bat = 74;

    lcdPrintArea(&topLine, "x: %dmm y: %dmm a: %d(%d)°\nc: %d%% t: %ds b: %d%%\n",
             x_mm, y_mm, a_deg, arel_deg, pos_cor, time, bat);             
    
        
    chThdSleepMilliseconds(100);
  }

  return 0;
}

void lcdInit(void)
{
  ili9341Init((uint8_t *)frame_buffer);

  //init ascii areas
  lcdPrintDriverVMT.put = lcdPutChar;
  topLine.vmt = &lcdPrintDriverVMT;
  topLine.startX = 0;
  topLine.startY = 0;
  topLine.maxRow = 2;
  topLine.maxCol = 45;
  topLine.rowModulo = 2;
  topLine.font = &TM_Font_7x10; 

  console.vmt = &lcdPrintDriverVMT;
  console.startX = 150;
  console.startY = 25;
  console.maxRow = 21;
  console.maxCol = 24;
  console.rowModulo = 1;
  console.font = &TM_Font_7x10; 

	uint32_t i;
	for (i = 0; i < ILI9341_PIXEL; i++) {
		*(uint16_t *) (frame_buffer + i) = 0x0000;
	}

  int x;
  int y;

  for(x = 0; x < 320; x += 1)
  {
    lcdSetPixel(x, 21, 0xFBE4);
  }

  for(y = 21; y < 240; y += 1)
  {
    lcdSetPixel(142, y, 0xFBE4);
  }

  double c = 200.0 / 300.0;
  for(y = 0; y < 300 * c; y += 1)
  {
    for(x = 0; x < 200 * c; x += 1)
    {
      lcdSetPixel(x, y + 30, 0x07E0);
    }
  }
  chThdCreateStatic(waLcdThread, sizeof(waLcdThread), LCD_SCHEDULER_PRIO, lcdThread, NULL);
}


void lcdPrintln(const char * fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  lcdvPrintArea(&console, fmt, ap);
  console.rowPosition += 1;
  console.colPosition = 0;

  va_end(ap);
}
