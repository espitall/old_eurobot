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
  usirInit();
  pathfinderInit();
  mecaInit();
  //gyroInit();
  stratInit();

  lcdPrintln(LCD_WARNING, "Start: robot secondaire");


  trajectorySetSafetymm(400);
  asservSetEnable(1);

  lcdPrintln(LCD_INFO, "Attente tapis");
  uint8_t left = 0;
  uint8_t right = 0;
  while(!left || !right)
  {
    uint16_t read = max7317Read();
    if(read & (1 << IO_SWITCH_BACK_LEFT))
    {
      mecaSetLeftCarpetState(MECA_CARPET_OPEN_1);
      lcdPrintln(LCD_INFO, "Tapis gauche: OK");
      left = 1;
    }

    if(read & (1 << IO_SWITCH_BACK_RIGHT))
    {
      mecaSetRightCarpetState(MECA_CARPET_OPEN_1);
      lcdPrintln(LCD_INFO, "Tapis droit: OK");
      right = 1;
    }
    chThdSleepMilliseconds(100);
  }

  lcdPrintln(LCD_INFO, "Attente tirette (mise en place)");

  while(!(max7317Read() & (1 << IO_SWITCH_STARTUP)))
  {
    chThdSleepMilliseconds(100);
  }
  mecaSetLeftCarpetState(MECA_CARPET_CLOSE);
  mecaSetRightCarpetState(MECA_CARPET_CLOSE);

  lcdPrintln(LCD_INFO, "Asserv: go");
  //asservSetEnable(1);
  //stratWedging();

  posSetYmm(1000);
  posSetXmm(-1100 + 90);
  posSetAdeg(0);

  lcdPrintln(LCD_INFO, "Attente du depart");

  while(max7317Read() & (1 << IO_SWITCH_STARTUP))
  {
    chThdSleepMilliseconds(100);
  }
  trajectorySetSafetymm(400);
  stratStart();

  while(true)
  {
    chThdSleepMilliseconds(3000);
  }

  return 0;
}
