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
#include <trajectory.h>
#include <usir.h>
#include <pathfinder.h>
#include "meca.h"

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
  trajectoryInit();
  asservInit();
  max11628Init();
  mecaInit();
  usirInit();
  pathfinderInit();
  //gyroInit();

  lcdPrintln(LCD_WARNING, "Start: robot principal");

 
  lcdPrintln(LCD_INFO, "Asserv: attente 3s");
  chThdSleepMilliseconds(3000);
  asservSetEnable(1);
  lcdPrintln(LCD_INFO, "Asserv: ok");


  int i = 0;
  int d = 400;
  while (true)
  {
    lcdPrintln(LCD_INFO, "tour: %d", i);
    i += 1;
    TRAJECTORY_XY_MM(d, 0);
    TRAJECTORY_XY_MM(d, -d);
    TRAJECTORY_XY_MM(0, -d);
    TRAJECTORY_XY_MM(0, 0);
    trajectoryWait();
  }
}
