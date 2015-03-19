#include <ch.h>
#include <hal.h>
#include <math.h>
#include <lcd.h>
#include <field.h>
#include <dc_motors.h>
#include <position.h>
#include <asserv.h>
#include <pcm9685.h>
#include <max11628.h>

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
  max11628Init();

  lcdPrintln("Start: robot principal");

  int i = 0;
  while (true)
  {
    dcmSetWidth(0, -i * 10);
    dcmSetWidth(1, 0);
    //test adc
    lcdPrintln("val %d",(int)(max11628ReadmV(15) * (5.1 + 1.0) / 1.0));

    //test pwm
    i += 10;
    i = i % 400;
    pcm9685SetChannel(0, 0, 230 + i);
    pcm9685SetChannel(1, 0, 230 + i);

    chThdSleepMilliseconds(100);
  }
}