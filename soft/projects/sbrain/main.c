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
#include <max7317.h>
#include <gyro.h>
#include <trajectory.h>
#include <usir.h>
#include <pathfinder.h>
#include "strat.h"

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
  dcmInit();
  pcm9685Init();
  max11628Init();
  max7317Init();
  lcdInit();
  fieldInit();
  posInit(position_computed_hook);
  trajectoryInit();
  asservInit();
  //mecaInit();
  //usirInit();
  pathfinderInit();
  //gyroInit();
  stratInit();

  lcdPrintln(LCD_WARNING, "Start: robot secondaire");

  pcm9685SetChannel(8, 0, 220);
  pcm9685SetChannel(9, 0, 500);
  pcm9685SetChannel(10, 0, 400);
  pcm9685SetChannel(11, 0, 550);
  while (true)
  {
    lcdPrintln(LCD_WARNING, "p1");
    pcm9685SetChannel(9, 0, 350);
    chThdSleepMilliseconds(5000);
    lcdPrintln(LCD_WARNING, "p2");
    pcm9685SetChannel(9, 0, 500);
    chThdSleepMilliseconds(5000);
  }
}
