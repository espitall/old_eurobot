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

  //draw circle (for test)
  a.x = 1500;
  a.y = 1050;
  lcdCircle(a, 150, 0, 360, LCD_COLOR(255, 0, 0), LCD_METRIC);
}
