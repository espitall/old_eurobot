#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <asserv.h>
#include <max7317.h>
#include <trajectory.h>
#include <position.h>
#include <step.h>
#include "strat.h"
#include "meca.h"

//#define TAKE_2_BALL

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
      mecaDisable();
      stepAction(STEP_ACTION_DISABLE);
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
  _color = STRAT_COLOR_YELLOW;
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

  TRAJECTORY_D_MM(-40);
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

  TRAJECTORY_D_MM(-40);
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

  TRAJECTORY_D_MM(-50);
  trajectoryWait();

  stepAction(STEP_ACTION_LEAVE_SPOT_LEFT);
  stepWait();
}

void stratLeaveSoftLeft(void)
{
  stepAction(STEP_ACTION_LEAVE_SOFT_SPOT_LEFT);
  stepWait();
}

void stratLeaveRight(void)
{
  stepAction(STEP_ACTION_PRETAKE_SPOT_RIGHT);
  stepWait();
  TRAJECTORY_D_MM(50);
  trajectoryWait();

  TRAJECTORY_D_MM(-50);
  trajectoryWait();

  stepAction(STEP_ACTION_LEAVE_SPOT_RIGHT);
  stepWait();
}

void stratLeaveSoftRight(void)
{
  stepAction(STEP_ACTION_LEAVE_SOFT_SPOT_RIGHT);
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

  asservSetEnable(1);

  trajectorySetSafetymm(0);
#ifdef TAKE_2_BALL
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stepWait();
      stepAction(STEP_ACTION_PRETAKE_FIRST_2BALL_RIGHT);
      stepWait();
      asservSetEnable(1);
      TRAJECTORY_D_MM_TEMP_S(20,2);
      TRAJECTORY_D_MM(-20);
      trajectoryWait();
      stepAction(STEP_ACTION_TAKE_FIRST_2BALL_RIGHT);
      stepWait();
      break;

    case STRAT_COLOR_GREEN:
      stepWait();
      stepAction(STEP_ACTION_PRETAKE_FIRST_2BALL_LEFT);
      stepWait();
      asservSetEnable(1);
      TRAJECTORY_D_MM_TEMP_S(20,2);
      TRAJECTORY_D_MM(-20);
      trajectoryWait();
      stepAction(STEP_ACTION_TAKE_FIRST_2BALL_LEFT);
      stepWait();
      break;
  }
#else
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stepWait();
      stepAction(STEP_ACTION_PRETAKE_FIRST_BALL_RIGHT);
      stepWait();
      asservSetEnable(1);
      TRAJECTORY_D_MM(20);
      TRAJECTORY_D_MM(-20);
      trajectoryWait();
      stepAction(STEP_ACTION_TAKE_FIRST_BALL_RIGHT);
      stepWait();
      break;

    case STRAT_COLOR_GREEN:
      stepWait();
      stepAction(STEP_ACTION_PRETAKE_FIRST_BALL_LEFT);
      stepWait();
      asservSetEnable(1);
      TRAJECTORY_D_MM(20);
      TRAJECTORY_D_MM(-20);
      trajectoryWait();
      stepAction(STEP_ACTION_TAKE_FIRST_BALL_LEFT);
      stepWait();
      break;
  }
#endif
  trajectorySetSafetymm(470);

  stepWait();
  TRAJECTORY_TEMP_S(3);
  TRAJECTORY_D_MM(-500);
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stepAction(STEP_ACTION_PREP_SPOT_RIGHT);
      break;

    case STRAT_COLOR_GREEN:
      stepAction(STEP_ACTION_PREP_SPOT_LEFT);
      break;
  }
  trajectoryWait();

  /*
  // homologation
  //TRAJECTORY_XY_MM(-630, 645);
  //TRAJECTORY_XY_MM(-400, 230);
  //TRAJECTORY_D_MM(-500);
  //trajectoryWait();

  //asservSetEnable(0);
   */

  if(1)
  {

    //take 1
    TRAJECTORY_XY_MM(-630, 645);
    trajectoryWait();
    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        stratTakeRight();
        break;

      case STRAT_COLOR_GREEN:
        stratTakeLeft();
        break;
    }

    asservSlow();
    //take 2
    //TRAJECTORY_XY_MM(-200, 600);
    //trajectoryWait();
    //stratTakeLeft();
    //TRAJECTORY_D_MM(-200);
    //trajectoryWait();

    //take 3
    trajectorySetSafetymm(200);
    TRAJECTORY_XY_MM(-400, 300);
    TRAJECTORY_D_MM(20);
    trajectoryWait();
    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        stratLeaveRight();
        break;

      case STRAT_COLOR_GREEN:
        stratLeaveLeft();
        break;
    }
    trajectorySetSafetymm(470);
    stepAction(STEP_ACTION_PREP_SPOT_CENTER);
    TRAJECTORY_D_MM(-500);
    trajectoryWait();
  }

 // asservSetEnable(0);

  asservNormal();
  TRAJECTORY_XY_BACK_MM(-1270, 230);
  stepWait();
  trajectoryWait();

  trajectorySetSafetymm(300);
 // TRAJECTORY_TEMP_S(2);
  trajectoryWait();
  double tempy = posGetYmm();
  trajectorySetSafetymm(0);
  asservSlow();
  TRAJECTORY_A_DEG_TEMP_S(0, 1.5);
  trajectoryStatus_t traj = trajectoryWait();
  if(traj != TRAJECTORY_OK)
  {

    asservNormal();
    TRAJECTORY_D_MM(400);
    traj = trajectoryWait();
    trajectorySetSafetymm(300);

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightArm(MECA_ARM_UP);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftArm(MECA_ARM_UP);
        break;
    }

  }
  else
  {
    TRAJECTORY_D_MM_TEMP_S(-40, 1.5);
    trajectoryStatus_t traj = trajectoryWait();
    trajectorySetSafetymm(300);

    switch(_color)
    {
      case STRAT_COLOR_YELLOW:
        mecaSetRightArm(MECA_ARM_UP);
        break;

      case STRAT_COLOR_GREEN:
        mecaSetLeftArm(MECA_ARM_UP);
        break;
    }

    if(traj != TRAJECTORY_OK)
    {
      TRAJECTORY_D_MM(30);
    }

  }

  asservNormal();
  TRAJECTORY_XY_MM(-600, tempy);
  trajectoryWait();

  TRAJECTORY_XY_BACK_MM(-720, tempy);
  TRAJECTORY_A_DEG(90);
  trajectoryWait();

  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      mecaSetRightArm(MECA_ARM_DOWN);
      break;

    case STRAT_COLOR_GREEN:
      mecaSetLeftArm(MECA_ARM_DOWN);
      break;
  }

  trajectorySetSafetymm(470);
  TRAJECTORY_XY_MM(-600, 600);
  trajectoryWait();

  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stepAction(STEP_ACTION_PREP_SPOT_LEFT);
      break;

    case STRAT_COLOR_GREEN:
      stepAction(STEP_ACTION_PREP_SPOT_LEFT);
      break;
  }

  TRAJECTORY_XY_MM(-200, 600);
  trajectoryWait();
  stepWait();

  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stratTakeLeft();
      break;

    case STRAT_COLOR_GREEN:
      stratTakeRight();
      break;
  }

  asservSlow();
  lcdPrintln(LCD_INFO,"Retour maison");
  TRAJECTORY_XY_MM_TEMP_S(-800, 1000, 5);
  trajectoryWait();
  lcdPrintln(LCD_INFO,"Lache");
  
  switch(_color)
  {
    case STRAT_COLOR_YELLOW:
      stratLeaveSoftLeft();
      break;

    case STRAT_COLOR_GREEN:
      stratLeaveSoftRight();
      break;
  }

  TRAJECTORY_D_MM(-100);
  trajectoryWait();

  asservSetEnable(0);
}

strat_color_t stratGetColor(void)
{
  return _color;
}
