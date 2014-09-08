#ifndef LCD_H
#define LCD_H

void lcdInit(void);
void lcdPrintln(const char * fmt, ...) __attribute__ ((format (printf, 1, 2)));;

#endif// LCD_H
