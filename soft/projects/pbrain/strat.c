#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <asserv.h>
#include <max7317.h>
#include <trajectory.h>
#include <position.h>
#include "strat.h"

static systime_t _startTime;
static strat_color_t _color;

static WORKING_AREA(waStratTime, 2048);

int stratGetTimeLeft()
{
  if(_startTime == 0)
  {
    return 90;
  }
  else
  {
    systime_t now = chTimeNow();
    if(now - _startTime > S2ST(90))
    {
      return 0;
    }
    return 90 - (now - _startTime) / S2ST(1);
  }
}

static msg_t stratTimeThread(void *arg) 
{
  (void)arg;
  while(1)
  {
    chThdSleepMilliseconds(200);
    if(stratGetTimeLeft() <= 1)
    {
      asservSetEnable(0);
    }
  }
  return 0;
}

void stratInit(void)
{
  _startTime = 0;

  chThdCreateStatic(waStratTime, sizeof(waStratTime), HIGHPRIO, stratTimeThread, NULL);

  if(max7317Read() & (1 << IO_SWITCH_COLOR))
  {
    _color = STRAT_COLOR_GREEN;
  }
  else
  {
    _color = STRAT_COLOR_YELLOW;
  }

  switch(_color)
  {
    case STRAT_COLOR_GREEN:
      lcdPrintln(LCD_INFO, "Couleur: VERT");
      break;

    case STRAT_COLOR_YELLOW:
      lcdPrintln(LCD_INFO, "Couleur: JAUNE");
      break;
  }
}

void stratWedging(void)
{
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(0);
  posSetXmm(-1500 + 105);

  TRAJECTORY_D_MM(200);
  TRAJECTORY_A_DEG(90);
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(90);
  posSetYmm(105);

  TRAJECTORY_D_MM(100);
  TRAJECTORY_XY_MM(-700, 1000);
  TRAJECTORY_A_DEG(0);
  TRAJECTORY_D_MM(-600);
}

void stratStart(void)
{
  _startTime = chTimeNow();
  lcdPrintln(LCD_INFO, "Asserv: go");
  asservSetEnable(1);

  stratWedging();
}

strat_color_t stratGetColor(void)
{
  return _color;
}
