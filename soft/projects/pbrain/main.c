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
#include <step.h>
#include <pathfinder.h>
#include "meca.h"
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
  mecaInit();
  //stepInit();
  //usirInit();
  pathfinderInit();
  //gyroInit();
  stratInit();

  lcdPrintln(LCD_WARNING, "Start: robot principal");
  /*
  posSetXmm(-500);
  posSetYmm(1000);

  pathfinderGotoXYmm(950, 250); // 250x950, 950x250
  */  //pathfinderGotoXYmm(850, 1650);
  //pathfinderGotoXYmm(-850, 1650);
  //pathfinderGotoXYmm(-950, 250);
  
  while(0)
  {
    while((max7317Read() & (1 << IO_SWITCH_BACK_LEFT)))
    {
      chThdSleepMilliseconds(100);
    }
    lcdPrintln(LCD_INFO, "Click !");
    stepAction(STEP_ACTION_TAKE_RIGHT);
    stepWait();
    lcdPrintln(LCD_INFO, "Done !");

    while(!(max7317Read() & (1 << IO_SWITCH_BACK_LEFT)))
    {
      chThdSleepMilliseconds(100);
    }
  }

  
  lcdPrintln(LCD_INFO, "Attente tirette (mise en place)");
  while(!(max7317Read() & (1 << IO_SWITCH_STARTUP)))
  {
    chThdSleepMilliseconds(100);
  }

  lcdPrintln(LCD_INFO, "Attente du depart");

  while(max7317Read() & (1 << IO_SWITCH_STARTUP))
  {
    chThdSleepMilliseconds(100);
  }

  stratStart();
  
  TRAJECTORY_D_MM(800);
  trajectoryWait();
  pathfinderGotoXYmm(950, 250);
  /*pathfinderGotoXYmm(850, 1650);
  pathfinderGotoXYmm(-850, 1650);
  pathfinderGotoXYmm(-950, 250);
  */
  //chThdSleepMilliseconds(3000);
  //  dcmSetWidth(0, 500);
  //  dcmSetWidth(1, -500);
  //while (true)
  //{
  //  lcdPrintln(LCD_INFO,"l %ld, r %ld", posGetLeftTick(), posGetRightTick());
  //  chThdSleepMilliseconds(500);
  //}

 
  //int i = 0;
  //int d = 400;
  //while (true)
  //{
  //  lcdPrintln(LCD_INFO, "tour: %d", i);
  //  i += 1;
  //  TRAJECTORY_XY_MM(d, 0);
  //  TRAJECTORY_XY_MM(d, d);
  //  TRAJECTORY_XY_MM(0, d);
  //  TRAJECTORY_XY_MM(0, 0);
  //  trajectoryWait();
  //}

  while(true)
  {
    chThdSleepMilliseconds(3000);
  }
}
