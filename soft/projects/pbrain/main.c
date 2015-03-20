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
#include <gyro.h>

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
  //gyroInit();

  lcdPrintln("Start: robot principal");

  
  lcdPrintln("Asserv: attente 3s");
  chThdSleepMilliseconds(3000);
  asservSetEnable(1);
  lcdPrintln("Asserv: ok");

  //dcmSetWidth (0, 800);
  //dcmSetWidth (1, -800);

  //int32_t distance;
  //int32_t angle;
  
  //int i = 0;
  while (true)
  {
    chThdSleepMilliseconds(10);
    /*
    dcmSetWidth(0,i);
    i += 10;*/
    /*
    distance = (int) posGetDmm();
    lcdPrintln("%ld mm", distance);
    */
    /*
    angle = (int) posGetAdeg();
    lcdPrintln("%ld deg", angle);
    */
  }
}
