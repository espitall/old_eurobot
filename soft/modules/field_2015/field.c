#include <lcd.h>
#include "field.h"

void fieldInit(void)
{
  //nothing to do
}

void fieldPrint(void)
{
  //print empty field
  lcd2DPoint_t a,b;

  //draw arena background
  a.x = 0;
  a.y = 0;
  b.x = 3000;
  b.y = 2000;
  lcdRect(a, b, LCD_COLOR(0, 255, 0), LCD_METRIC);
}
