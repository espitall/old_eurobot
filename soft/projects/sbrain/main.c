#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <field.h>


int main(void) 
{
  //RTOS initialization
  halInit();
  chSysInit();

  palSetPad(GPIOG, GPIOG_LED4_RED);

  //modules and peripherals initialization
  lcdInit();
  fieldInit();

  palSetPad(GPIOG, GPIOG_LED3_GREEN);


  //background task: lcd console test
  SDRAM int i;
  int j;
  i = 0;
  j = 0;
  while (TRUE) {
    chThdSleepMilliseconds(500);
    palSetPad(GPIOG, GPIOG_LED4_RED);
    lcdPrintln("plop %d %d 0x%lX", i++, j++, (uint32_t)&i);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOG, GPIOG_LED4_RED);
  }

}
