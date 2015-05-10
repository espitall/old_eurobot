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
#include "meca.h"

void position_computed_hook(void)
{
  asservCompute();
}

void testInput(void)
{
  while(true)
  {
    uint16_t read = max7317Read();
    lcdPrintln(LCD_INFO, "0x%04x tirette:%d swg:%d swd:%d color%d", 
               read,
               !!(read & (1 << IO_SWITCH_STARTUP)),
               !!(read & (1 << IO_SWITCH_BACK_LEFT)),
               !!(read & (1 << IO_SWITCH_BACK_RIGHT)),
               !!(read & (1 << IO_SWITCH_COLOR)));
    chThdSleepMilliseconds(100);
  }
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
  mecaInit();
  //gyroInit();
  stratInit();

  lcdPrintln(LCD_WARNING, "Start: robot secondaire");

  testInput();
  

  lcdPrintln(LCD_INFO, "Attente tapis");
  uint8_t left = 0;
  uint8_t right = 0;
  while(!left || !right)
  {
    uint16_t read = max7317Read();
    if(read & (1 << IO_SWITCH_BACK_LEFT))
    {
      mecaSetLeftCarpetState(MECA_CARPET_OPEN_1);
    }

    if(read & (1 << IO_SWITCH_BACK_RIGHT))
    {
      mecaSetRightCarpetState(MECA_CARPET_OPEN_1);
    }
    chThdSleepMilliseconds(100);
  }

  lcdPrintln(LCD_INFO, "Attente tirette (mise en place)");

  //pcm9685SetChannel(8, 0, 220);
  //pcm9685SetChannel(9, 0, 500);
  //pcm9685SetChannel(10, 0, 400);
  //pcm9685SetChannel(11, 0, 550);
  while (true)
  {
    lcdPrintln(LCD_WARNING, "High");
    mecaSetBackFoot(MECA_FOOT_HIGH);
    chThdSleepMilliseconds(5000);

    lcdPrintln(LCD_WARNING, "Low");
    mecaSetBackFoot(MECA_FOOT_LOW);
    chThdSleepMilliseconds(5000);
  }
}
