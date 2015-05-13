#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <asserv.h>
#include <max7317.h>
#include <trajectory.h>
#include <position.h>
#include <step.h>
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
    _color = STRAT_COLOR_YELLOW;
  }
  else
  {
    _color = STRAT_COLOR_GREEN;
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
  TRAJECTORY_A_DEG(18180);
  TRAJECTORY_D_MM(600);
}

void stratTakeLeft(void)
{
  stepAction(STEP_ACTION_PRETAKE_SPOT_LEFT);
  stepWait();
  TRAJECTORY_D_MM(50);
  trajectoryWait();

  TRAJECTORY_D_MM(-50);
  trajectoryWait();

  stepAction(STEP_ACTION_TAKE_SPOT_LEFT);
  stepWait();
}

void stratTakeRight(void)
{
  stepAction(STEP_ACTION_PRETAKE_SPOT_RIGHT);
  stepWait();
  TRAJECTORY_D_MM(50);
  trajectoryWait();

  TRAJECTORY_D_MM(-50);
  trajectoryWait();

  stepAction(STEP_ACTION_TAKE_SPOT_RIGHT);
  stepWait();
}

void stratLeaveLeft(void)
{
  stepAction(STEP_ACTION_PRETAKE_SPOT_LEFT);
  stepWait();
  TRAJECTORY_D_MM(50);
  trajectoryWait();

  TRAJECTORY_D_MM(-40);
  trajectoryWait();

  stepAction(STEP_ACTION_LEAVE_SPOT_LEFT);
  stepWait();
}

void stratLeaveRight(void)
{
  stepAction(STEP_ACTION_PRETAKE_SPOT_RIGHT);
  stepWait();
  TRAJECTORY_D_MM(50);
  trajectoryWait();

  TRAJECTORY_D_MM(-40);
  trajectoryWait();

  stepAction(STEP_ACTION_LEAVE_SPOT_RIGHT);
  stepWait();
}

void stratWedgingPreClap(void)
{
  stepAction(STEP_ACTION_PREP_SPOT_RIGHT);
  TRAJECTORY_D_MM(-200);
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(90);
  posSetYmm(105);

  TRAJECTORY_D_MM(450);
  TRAJECTORY_A_DEG(0);
  TRAJECTORY_D_MM(-450);
  TRAJECTORY_WEDGING();
  trajectoryWait();
  posSetAdeg(0);
  posSetXmm(-1500 + 105);

  TRAJECTORY_D_MM(130);
  TRAJECTORY_A_DEG(-90);

  TRAJECTORY_D_MM(150);
  trajectoryWait();
  stratTakeRight();
  TRAJECTORY_D_MM(100);
  trajectoryWait();
  stratTakeRight();

  TRAJECTORY_D_MM(-300);
  TRAJECTORY_A_DEG(0);
  TRAJECTORY_D_MM(300);
  TRAJECTORY_XY_MM(-1300, 1000);
  TRAJECTORY_XY_MM(-1100, 1000);
  stratLeaveRight();
  TRAJECTORY_D_MM(-500);
  trajectoryWait();

}

void stratStart(void)
{
  _startTime = chTimeNow();

  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stepAction(STEP_ACTION_TAKE_FIRST_BALL_LEFT);
      break;

    case STRAT_COLOR_GREEN:
      stepAction(STEP_ACTION_TAKE_FIRST_BALL_RIGHT);
      break;
  }
  stepWait();
  TRAJECTORY_D_MM(-500);
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stepAction(STEP_ACTION_PREP_SPOT_LEFT);
      break;

    case STRAT_COLOR_GREEN:
      stepAction(STEP_ACTION_PREP_SPOT_RIGHT);
      break;
  }
  trajectoryWait();

  TRAJECTORY_XY_MM(-630, 645);
  TRAJECTORY_XY_MM(-400, 230);
  TRAJECTORY_D_MM(-500);
  trajectoryWait();

  asservSetEnable(0);

  if(1)
  {

    //take 1
    TRAJECTORY_XY_MM(-630, 645);
    trajectoryWait();
    stratTakeLeft();

    asservSlow();
    //take 2
    TRAJECTORY_XY_MM(-200, 600);
    trajectoryWait();
    stratTakeLeft();
    TRAJECTORY_D_MM(-200);
    trajectoryWait();

    //take 3
    TRAJECTORY_XY_MM(-400, 300);
    trajectoryWait();
    stratLeaveLeft();
    TRAJECTORY_D_MM(-500);
  }

  TRAJECTORY_XY_MM(-881, 400);
  TRAJECTORY_A_DEG(90);
  trajectoryWait();

  stratWedgingPreClap();
  asservSetEnable(0);


}

strat_color_t stratGetColor(void)
{
  return _color;
}
