#include <ch.h>
#include <hal.h>
#include <math.h>
#include <lcd.h>
#include <field.h>
#include <dc_motors.h>
#include <position.h>
#include <asserv.h>
#include <pcm9685.h>

void position_computed_hook(void)
{
  asservCompute();
}

int main(void) 
{
  //RTOS initialization
  halInit();
  chSysInit();

  palSetPad(GPIOG, GPIOG_LED4_RED);

  //modules and peripherals initialization
  pcm9685Init();
  dcmInit();
  lcdInit();
  fieldInit();
  posInit(position_computed_hook);
  asservInit();

  lcdPrintln(LCD_INFO, "Start");


 //     dcmSetWidth(0, 5000);
 //     dcmSetWidth(1, 2500);
  /*
  int pwm[2] = {0, 5000};
  int sens[2] = {1, 1};
  while (TRUE) 
  {
    int i;
    for(i = 0; i < 2; i += 1)
    {
      pwm[i] += 100 * sens[i];
      if(fabs(pwm[i]) > 8000)
      {
        sens[i] *= -1;
      }
      dcmSetWidth(0, 5000);
      dcmSetWidth(1, 2500);

    }
    lcdPrintln("pwm: %d %d", pwm[0], pwm[1]);
    chThdSleepMilliseconds(500);
  }
  */

  int i = 0;
  while(TRUE)
  {
  pcm9685SetChannel(0, 0, 230 + i);
  pcm9685SetChannel(1, 0, 230 + i);

  i += 10;
  i = i % 400;

    chThdSleepMilliseconds(100);
  }
}
