#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <field.h>
#include <dc_motors.h>


int main(void) 
{
  //RTOS initialization
  halInit();
  chSysInit();

  palSetPad(GPIOG, GPIOG_LED4_RED);

  //modules and peripherals initialization
  dcmInit();
  lcdInit();
  fieldInit();

  //test moteur
  int pwm = 0;
  int sens = 1;
  while (TRUE) {
    pwm += 1000 * sens;
    if(pwm > 8000)
    {
      sens = -1;
    }
    if(pwm < -8000)
    {
      sens = 1;
    }
    lcdPrintln("test moteur pwm: %d", pwm);
    dcmSetWidth(0, pwm);
    chThdSleepMilliseconds(500);
  }

  while(TRUE);
}
