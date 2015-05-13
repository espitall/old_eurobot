#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <asserv.h>
#include <max7317.h>
#include <trajectory.h>
#include <position.h>
#include "strat.h"
#include "meca.h"

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
  lcdPrintln(LCD_INFO, "Calage start");
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(-90);
  posSetYmm(2000 - 80);

  lcdPrintln(LCD_INFO, "Calage phase 2");
  TRAJECTORY_D_MM(200);
  TRAJECTORY_A_DEG(0);
  TRAJECTORY_D_MM(-100);
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(0);
  posSetXmm(-1500 - 80);

  lcdPrintln(LCD_INFO, "Calage phase 3");
  TRAJECTORY_D_MM(100);
  TRAJECTORY_XY_MM(-700, 1000);
  TRAJECTORY_XY_BACK_MM(-1100, 1000);
  TRAJECTORY_A_DEG(0);
  trajectoryWait();
}

void stratStart(void)
{
  _startTime = chTimeNow();
  lcdPrintln(LCD_INFO, "Asserv: go");
  asservSetEnable(1);

  lcdPrintln(LCD_INFO, "Positionnement contre marches");
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetRightCarpetState(MECA_CARPET_OPEN_1);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetLeftCarpetState(MECA_CARPET_OPEN_1);
      break;
  }
  TRAJECTORY_XY_MM(-670, 1600);
  trajectoryWait();
  lcdPrintln(LCD_INFO, "Positionnement contre marches: OK");

  lcdPrintln(LCD_INFO, "Depose tapis");
  TRAJECTORY_XY_BACK_MM(-670, 1300);
  trajectoryWait();
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetRightCarpetState(MECA_CARPET_OPEN_2);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetLeftCarpetState(MECA_CARPET_OPEN_2);
      break;
  }
  TRAJECTORY_XY_MM(-655, 1600);
  TRAJECTORY_A_DEG(80);
  trajectoryWait();
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetRightCarpetState(MECA_CARPET_OPEN_3);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetLeftCarpetState(MECA_CARPET_OPEN_3);
      break;
  }
  chThdSleepMilliseconds(250);
  lcdPrintln(LCD_INFO, "Depose tapis: OK");

  asservSetEnable(0);

  lcdPrintln(LCD_INFO, "Recal");
  TRAJECTORY_A_DEG(80);
  TRAJECTORY_D_MM(-50);
  TRAJECTORY_A_DEG(180);
  TRAJECTORY_WEDGING();
  trajectoryWait();
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetRightCarpetState(MECA_CARPET_CLOSE);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetLeftCarpetState(MECA_CARPET_CLOSE);
      break;
  }
  posSetAdeg(180);
  posSetXmm(-533 - 80);
  double cy = posGetYmm();
  TRAJECTORY_D_MM(300);
  TRAJECTORY_XY_MM(-1050, cy);
  TRAJECTORY_XY_BACK_MM(-1050, cy + 300);
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(-90);
  posSetYmm(2000 - 80);
  TRAJECTORY_D_MM(200);
  trajectoryWait();
  lcdPrintln(LCD_INFO, "Recal: OK");
  

  lcdPrintln(LCD_INFO, "Prise simple");
  TRAJECTORY_XY_BACK_MM(-1200, 1800);
  TRAJECTORY_A_DEG(0);
  TRAJECTORY_TEMP_S(1);
  trajectoryWait();
  TRAJECTORY_D_MM(-120);
  TRAJECTORY_TEMP_S(3);
  chThdSleepMilliseconds(500);
  mecaSetSuckerState(2, MECA_SUCKER_ON);
  trajectoryWait();
  lcdPrintln(LCD_INFO, "Prise simple: OK");

  
  lcdPrintln(LCD_INFO, "Prise duo");
  TRAJECTORY_D_MM(500);
  trajectoryWait();
  mecaSetSuckerState(0, MECA_SUCKER_ON);
  mecaSetSuckerState(1, MECA_SUCKER_ON);
  TRAJECTORY_TEMP_S(1.5);
  trajectoryWait();
  lcdPrintln(LCD_INFO, "Prise duo: OK");

  
  lcdPrintln(LCD_INFO, "Depose");
  TRAJECTORY_XY_MM(-1300, 1000);
  TRAJECTORY_A_DEG(0);
  TRAJECTORY_D_MM(-300);
  TRAJECTORY_D_MM(300);
  TRAJECTORY_A_DEG(180);
  TRAJECTORY_D_MM(200);
  TRAJECTORY_D_MM(-300);
  trajectoryWait();
  lcdPrintln(LCD_INFO, "Depose: OK");

}

strat_color_t stratGetColor(void)
{
  return _color;
}
