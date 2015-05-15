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
    if(stratGetTimeLeft() <= 2)
    {
      asservSetEnable(0);

      switch(_color)
      {
        case STRAT_COLOR_YELLOW:
          mecaSetRightCarpetState(MECA_CARPET_OPEN_3);
          break;

        case STRAT_COLOR_GREEN:
          mecaSetLeftCarpetState(MECA_CARPET_OPEN_3);
          break;
      }
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


  trajectorySetSafetymm(0);
  lcdPrintln(LCD_INFO, "Positionnement contre marches");
  
  TRAJECTORY_XY_MM(-670, 1450);
  trajectoryWait();
  lcdPrintln(LCD_INFO, "Positionnement contre marches: OK");

  

  TRAJECTORY_TEMP_S(10);
  trajectorySetSafetymm(250);
  lcdPrintln(LCD_INFO, "Depose tapis");
  TRAJECTORY_XY_BACK_MM(-665, 1300);
  TRAJECTORY_A_DEG(-90);
  trajectoryWait();

  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetLeftCarpetState(MECA_CARPET_OPEN_1);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetRightCarpetState(MECA_CARPET_OPEN_1);
      break;
  }

  chThdSleepMilliseconds(750);


  int i;
  for(i = 0; i < 3; i += 1)
  {
    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetLeftCarpetState(MECA_CARPET_OPEN_1);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetRightCarpetState(MECA_CARPET_OPEN_1);
        break;
    }
    chThdSleepMilliseconds(250);

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetLeftCarpetState(MECA_CARPET_OPEN_2_BACK);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetRightCarpetState(MECA_CARPET_OPEN_2_BACK);
        break;
    }
    chThdSleepMilliseconds(250);
  }


  TRAJECTORY_XY_BACK_MM(-655, 1600);
 // TRAJECTORY_A_DEG(-80);
  trajectoryWait();
  for(i = 0; i < 3; i += 1)
  {
    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetLeftCarpetState(MECA_CARPET_OPEN_2_BACK);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetRightCarpetState(MECA_CARPET_OPEN_2_BACK);
        break;
    }

    chThdSleepMilliseconds(250);

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetLeftCarpetState(MECA_CARPET_OPEN_3);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetRightCarpetState(MECA_CARPET_OPEN_3);
        break;
    }
    chThdSleepMilliseconds(250);
  }

  lcdPrintln(LCD_INFO, "Depose tapis: OK");

  asservSetEnable(0);

  trajectorySetSafetymm(400);
  TRAJECTORY_XY_MM(-655, 1400);
  TRAJECTORY_XY_MM(-800, 1200);
  TRAJECTORY_XY_MM(-800, 1000);
  trajectoryWait();
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetLeftCarpetState(MECA_CARPET_CLOSE);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetRightCarpetState(MECA_CARPET_CLOSE);
      break;
  }

  TRAJECTORY_XY_MM(-210, 1000);
  TRAJECTORY_XY_MM(-210, 1250);
  trajectoryWait();

  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetRightCarpetState(MECA_CARPET2_OPEN_1);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetLeftCarpetState(MECA_CARPET2_OPEN_1);
      break;
  }
  TRAJECTORY_A_DEG(90);
  trajectoryWait();

  TRAJECTORY_D_MM_TEMP_S(130, 4);
  trajectoryWait();


  for(i = 0; i < 20; i += 1)
  {
    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightCarpetState(MECA_CARPET2_OPEN_1);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftCarpetState(MECA_CARPET2_OPEN_1);
        break;
    }

    if((i > 5) && (i < 15))
    {
      chThdSleepMilliseconds(500);
    }
    else
    {
      chThdSleepMilliseconds(250);
    }

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightCarpetState(MECA_CARPET2_OPEN_2);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftCarpetState(MECA_CARPET2_OPEN_2);
        break;
    }
    if((i > 5) && (i < 15))
    {
      chThdSleepMilliseconds(500);
    }
    else
    {
      chThdSleepMilliseconds(250);
    }
  }
  for(i = 0; i < 5; i += 1)
  {

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightCarpetState(MECA_CARPET2_OPEN_2);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftCarpetState(MECA_CARPET2_OPEN_2);
        break;
    }
    chThdSleepMilliseconds(250);

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightCarpetState(MECA_CARPET2_OPEN_3);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftCarpetState(MECA_CARPET2_OPEN_3);
        break;
    }
    chThdSleepMilliseconds(250);
  }

  for(i = 0; i < 5; i += 1)
  {

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightCarpetState(MECA_CARPET2_CLOSE);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftCarpetState(MECA_CARPET2_CLOSE);
        break;
    }
    chThdSleepMilliseconds(250);

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightCarpetState(MECA_CARPET2_OPEN_3);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftCarpetState(MECA_CARPET2_OPEN_3);
        break;
    }
    chThdSleepMilliseconds(250);
  }
  posSetAdeg(90);

  TRAJECTORY_D_MM(-400);

  TRAJECTORY_A_DEG(165);
  TRAJECTORY_D_MM(-150);
  TRAJECTORY_A_DEG(90);
  TRAJECTORY_D_MM_TEMP_S(300, 10);
  TRAJECTORY_D_MM_TEMP_S(100, 3);
  trajectoryWait();
  posSetAdeg(90);
  
  for(i = 0; i < 3; i += 1)
  {
    TRAJECTORY_D_MM_TEMP_S(-100, 2);
    TRAJECTORY_D_MM_TEMP_S(120, 2);
  }
  trajectoryWait();
  TRAJECTORY_D_MM(-150);
  TRAJECTORY_XY_MM(-210, 1000);
  trajectoryWait();


  /*
  mecaSetSuckerState(1, MECA_SUCKER_ON);
  TRAJECTORY_TEMP_S(2);
  TRAJECTORY_XY_MM_TEMP_S(-695, 1800, 3);
  TRAJECTORY_XY_BACK_MM(-900, 1000);
  TRAJECTORY_XY_BACK_MM(-1250, 1000);
  TRAJECTORY_XY_BACK_MM(-1250, 1000);
  TRAJECTORY_TEMP_S(2);
  trajectoryWait();
  mecaSetSuckerState(1, MECA_SUCKER_OFF);

  */


  /*
  TRAJECTORY_XY_BACK_MM(-1110, 1800);
  trajectoryWait();
  mecaSetSuckerState(2, MECA_SUCKER_ON);
  TRAJECTORY_XY_BACK_MM(-1330, 1800);
  TRAJECTORY_D_MM_TEMP_S(-10, 1.5);
  trajectoryWait();
  mecaSetSuckerState(2, MECA_SUCKER_LOCKED);
  TRAJECTORY_XY_MM(-800, 1760);
  trajectoryWait();
  mecaSetSuckerState(1, MECA_SUCKER_ON);
  TRAJECTORY_XY_MM(-600, 1760);
  TRAJECTORY_D_MM(-100);

  TRAJECTORY_XY_MM(-700, 1000);
  trajectoryWait();
  */

  asservSetEnable(0);



  /*
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

  */
}

strat_color_t stratGetColor(void)
{
  return _color;
}
