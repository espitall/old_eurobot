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

int[][] fieldMatrix()
{
  int x = FIELD_X / FIELD_RES;
  int y = FIELD_Y / FIELD_RES;
  int matrix[x][y];

  for (i = 0; i < x; i++)
  {
    for (j = 0; j < y; j++)
    {
      matrix[i][j] = 1;
    }
  }

  // escalier
  for (i = 100; i < 150; i++)
  {
    for (j = 80; j < 90; j++)
    {
      
    }
  }

  return matrix;
}
