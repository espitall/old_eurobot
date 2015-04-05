#ifndef LCD_H
#define LCD_H

#include <stdint.h>


//RGB to binary color macro
#define LCD_COLOR(r,g,b) (((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff) | 0xFF000000)

//flags
#define LCD_PIXEL  0x0000
#define LCD_METRIC 0x0001
#define LCD_FIELD  0x0002

typedef struct 
{
  int32_t x;
  int32_t y;
} lcd2DPoint_t;

typedef enum 
{
  LCD_INFO = 0x0,
  LCD_WARNING = 0x01,
  LCD_ERROR = 0x02,
} lcd_level_t;


/*
 * Initialization of the field module
 */
void lcdInit(void);

/*
 * Print text in the console (lcd). Syntax is like strandard printf
 */
void lcdPrintln(lcd_level_t lvl, const char * fmt, ...) __attribute__ ((format (printf, 2, 3)));;


/* ------ DRAWING functins ------ */

/*
 * Draw a rectangle on lcd
 */
void lcdRect(lcd2DPoint_t origin, lcd2DPoint_t dest, uint32_t color, uint16_t flags);

/*
 * Draw a full circle on lcd
 */
void lcdCircle(lcd2DPoint_t origin, int32_t rayon, int32_t start_angle, int32_t end_angle, uint32_t color, uint16_t flags);

#endif// LCD_H
