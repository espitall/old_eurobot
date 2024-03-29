#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <chprintf.h>
#include <field.h>
#include <position.h>
#include <trajectory.h>
#include <strat.h>

#include "lcd.h"
#include "ili9341.h"
#include "tm_fonts.h"
#include "config.h"
#include "max11628.h"

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
  uint32_t color;
} lcdPrintDriver_t;

static uint32_t SDRAM frame_buffer[ILI9341_PIXEL * 2];

struct BaseSequentialStreamVMT lcdPrintDriverVMT;
lcdPrintDriver_t topLine, console;
uint8_t current_layer;
static Mutex mutex;

static WORKING_AREA(waLcdThread, 4096);


static void lcdSetPixel(uint8_t layer, uint16_t x, uint16_t y, uint32_t color)
{
  if((x < ILI9341_HEIGHT) && (y < ILI9341_WIDTH))
  {
    frame_buffer[(ILI9341_HEIGHT - x) * ILI9341_WIDTH + y + ILI9341_PIXEL * layer] = color;
  }
}

static uint32_t lcdGetPixel(uint8_t layer, uint16_t x, uint16_t y)
{
  if((x < ILI9341_HEIGHT) && (y < ILI9341_WIDTH))
  {
    return frame_buffer[(ILI9341_HEIGHT - x) * ILI9341_WIDTH + y + ILI9341_PIXEL * layer];
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
          lcdSetPixel(current_layer, x + drv->startX, y + drv->startY, lcdGetPixel(current_layer, x + drv->startX, y + drv->startY + drv->font->FontHeight));
        }
        else
        {
          lcdSetPixel(current_layer, x + drv->startX, y + drv->startY, 0x00);
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
        lcdSetPixel(current_layer, x + drv->startX, y + drv->startY + drv->font->FontHeight * drv->rowPosition, 0x0000);
      }
    }
  }

  uint16_t xpos = drv->startX + drv->colPosition * drv->font->FontWidth;
  uint16_t ypos = drv->startY + drv->rowPosition * drv->font->FontHeight;

  if(c == LCD_INFO)
  {
    drv->color = LCD_COLOR(255, 255, 255);
    return 0;
  }
  if(c == LCD_WARNING)
  {
    drv->color = LCD_COLOR(255, 255, 0);
    return 0;
  }
  if(c == LCD_ERROR)
  {
    drv->color = LCD_COLOR(255, 0, 0);
    return 0;
  }


  if(c != '\n') 
  {
    
    uint16_t i,j,b;
    for (i = 0; i < drv->font->FontHeight; i++) 
    {
      if(c > 127)
      {
        c = '?';
      }
      b = drv->font->data[(c - 32) * drv->font->FontHeight + i];
      for (j = 0; j < drv->font->FontWidth; j++) 
      {
        if ((b << j) & 0x8000) 
        {
          lcdSetPixel(current_layer, xpos + j, ypos + i, drv->color);
        } 
        else
        {
          lcdSetPixel(current_layer, xpos + j, ypos + i, 0x0000);
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

static void lcdPrintArea(lcdPrintDriver_t * drv, const char * fmt, ...)  __attribute__ ((format (printf, 2, 3)));
static void lcdPrintArea(lcdPrintDriver_t * drv, const char * fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  lcdvPrintArea(drv, fmt, ap);

  va_end(ap);
}

static msg_t lcdThread(void *arg) 
{
  (void) arg;

  current_layer = 0;
  int x;
  int y;

  chMtxLock(&mutex);
  //fill black
  uint32_t i;
	for (i = 0; i < ILI9341_PIXEL; i++) 
  {
		*(frame_buffer + i) = 0x0000;
	}

  //draw separator
  for(x = 0; x < 320; x += 1)
  {
    lcdSetPixel(0, x, 21, LCD_COLOR(0x00, 0xFB, 0xE4));
  }
  for(y = 21; y < 240; y += 1)
  {
    lcdSetPixel(0, 142, y, LCD_COLOR(0x00, 0xFB, 0xE4));
  }
  chMtxUnlock();

  while (TRUE) {
    
    //copy current layer to next layer
    chMtxLock(&mutex);
    if(current_layer)
    {
      for (i = 0; i < ILI9341_PIXEL; i++) 
      {
        *(frame_buffer + i) = *(frame_buffer + i + ILI9341_PIXEL);
      }
    }
    else 
    {
      for (i = 0; i < ILI9341_PIXEL; i++) 
      {
        *(frame_buffer + i + ILI9341_PIXEL) = *(frame_buffer + i);
      }
    }
    current_layer = !current_layer;
    chMtxUnlock();

    int32_t x_mm = posGetXmm();
    int32_t y_mm = posGetYmm();
    int32_t a_deg = posGetAdeg();
    int32_t arel_deg = a_deg % 360;
    static uint8_t pos_cor = -1;
    pos_cor += 1;
    uint8_t time = stratGetTimeLeft();
    double bat = max11628ReadmV(15) * (5.1 + 1.0);
    uint16_t bat_decimal = (int) (bat / 1000.0);
    uint16_t bat_float = (int) (bat) % 1000;
    lcd_level_t bat_lvl = LCD_INFO;
    if(bat < BAT_ERROR_THRESHOLD_MV)
    {
      bat_lvl = LCD_ERROR;
    }
    else if(bat < BAT_WARNING_THRESHOLD_MV)
    {
      bat_lvl = LCD_WARNING;
    }

    lcdPrintArea(&topLine, "x: %ldmm y: %ldmm a: %ld (%ld)deg\nc: %d%% t: %ds b: %c%d.%dV%c     \n", x_mm, y_mm, a_deg, arel_deg, pos_cor, time, bat_lvl, bat_decimal, bat_float,LCD_INFO);
    
    fieldPrint();
    posPrint();
    trajectoryPrint();

    ili9341SetLayer(current_layer);
    chThdSleepMilliseconds(50);
  }

  return 0;
}

void lcdInit(void)
{
  chMtxInit(&mutex);

  ili9341Init((uint8_t *)frame_buffer);

  current_layer = 0;

  //init ascii areas
  lcdPrintDriverVMT.put = lcdPutChar;
  topLine.vmt = &lcdPrintDriverVMT;
  topLine.startX = 0;
  topLine.startY = 0;
  topLine.maxRow = 2;
  topLine.maxCol = 45;
  topLine.rowModulo = 2;
  topLine.font = &TM_Font_7x10; 
  topLine.color = LCD_COLOR(255, 255, 255);

  console.vmt = &lcdPrintDriverVMT;
  console.startX = 150;
  console.startY = 25;
  console.maxRow = 21;
  console.maxCol = 24;
  console.rowModulo = 1;
  console.font = &TM_Font_7x10; 

	
  chThdCreateStatic(waLcdThread, sizeof(waLcdThread), LCD_SCHEDULER_PRIO, lcdThread, NULL);
}


void lcdPrintln(lcd_level_t lvl, const char * fmt, ...) __attribute__ ((format (printf, 2, 3)));
void lcdPrintln(lcd_level_t lvl, const char * fmt, ...)
{
  chMtxLock(&mutex);
  va_list ap;

  va_start(ap, fmt);

  switch(lvl)
  {
    case LCD_INFO:
      console.color = LCD_COLOR(255, 255, 255);
      break;

    case LCD_WARNING:
      console.color = LCD_COLOR(255, 255, 0);
      break;

    case LCD_ERROR:
      console.color = LCD_COLOR(255, 0, 0);
      break;
  }

  lcdvPrintArea(&console, fmt, ap);
  console.rowPosition += 1;
  console.colPosition = 0;

  va_end(ap);
  chMtxUnlock();
}

void lcdRect(lcd2DPoint_t origin, lcd2DPoint_t dest, uint32_t color, uint16_t flags)
{
  if(flags & LCD_METRIC)
  {

    if(flags & LCD_FIELD)
    {
      origin.x += 1500;
      dest.x += 1500;
    }

    const double ratio = 200.0 / 3000.0;
    const double xoffset = 2;
    const double yoffset = 30;
    lcd2DPoint_t tmp;

    tmp.x = origin.x;
    tmp.y = origin.y;
    origin.x = tmp.y * ratio + xoffset;
    origin.y = tmp.x * ratio + yoffset;

    tmp.x = dest.x;
    tmp.y = dest.y;
    dest.x = tmp.y * ratio + xoffset;
    dest.y = tmp.x * ratio + yoffset;
  }

  chMtxLock(&mutex);
  int32_t i,j;
  for(i = origin.x; i < dest.x; i += 1)
  {
    for(j = origin.y; j < dest.y; j += 1)
    {
      lcdSetPixel(current_layer, i, j, color);
    }
  }
  chMtxUnlock();
}

void lcdCircle(lcd2DPoint_t origin, int32_t rayon, int32_t start_angle, int32_t end_angle, uint32_t color, uint16_t flags)
{
  if(flags & LCD_METRIC)
  {

    if(flags & LCD_FIELD)
    {
      origin.x += 1500;
    }

    const double ratio = 200.0 / 3000.0;
    const double xoffset = 2;
    const double yoffset = 30;
    lcd2DPoint_t tmp;

    tmp.x = origin.x;
    tmp.y = origin.y;
    origin.x = tmp.y * ratio + xoffset;
    origin.y = tmp.x * ratio + yoffset;

    rayon = rayon * ratio;
  }

  (void)start_angle;
  (void)end_angle;

  int32_t i,j;
  int32_t rayon_squared = rayon * rayon;

  chMtxLock(&mutex);
  for(i = -rayon; i < rayon; i += 1)
  {
    for(j = -rayon; j < rayon; j += 1)
    {
      if(i * i + j * j < rayon_squared)
      {
        lcdSetPixel(current_layer, origin.x + i, origin.y + j, color);
      }
    }
  }
  chMtxUnlock();
}
