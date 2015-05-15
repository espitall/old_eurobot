#include <ch.h>
#include <hal.h>
#include "servo.h"
#include "stepper.h"
#include "step.h"

static WORKING_AREA(waHeartbeatThread, 256);

msg_t heartbeatThread(void* arg) 
{
  (void)arg;

  while (1)
  {
    palSetPad(GPIOA, GPIOA_LED);
    chThdSleepMilliseconds(500);

    palClearPad(GPIOA, GPIOA_LED);
    chThdSleepMilliseconds(500);
  }

  return 0;
}

void initMeca(void)
{
  servoSet(SERVO_CLAMP_LEFT, 1000);
  servoSet(SERVO_CLAMP_RIGHT, 1000);
}

void handleStepActionTakeRight(void)
{
  servoSet(SERVO_CLAMP_RIGHT, 1300);
  chThdSleepMilliseconds(200);
  stepperSetTarget(0, 300);
  chThdSleepMilliseconds(200);

  stepperSetTarget(2, 300);
  chThdSleepMilliseconds(200);


  servoSet(SERVO_CLAMP_LEFT, 1300);
  chThdSleepMilliseconds(200);
  stepperSetTarget(1, 300);
  chThdSleepMilliseconds(200);

  stepperSetTarget(3, 300);
  chThdSleepMilliseconds(200);


  servoSet(SERVO_CLAMP_RIGHT, 1000);
  chThdSleepMilliseconds(200);
  stepperSetTarget(0, 0);
  chThdSleepMilliseconds(200);

  stepperSetTarget(2, 0);
  chThdSleepMilliseconds(200);



  servoSet(SERVO_CLAMP_LEFT, 1000);
  chThdSleepMilliseconds(200);
  stepperSetTarget(1, 0);
  chThdSleepMilliseconds(200);

  stepperSetTarget(3, 0);
  chThdSleepMilliseconds(200);
}

void handleStepActionReset(int yellow)
{
  //close clamps
  servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
  servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
  
  //reset lifts & sliders
  stepperSetPosition(STEP_LEFT_LIFT_ID, 0);
  stepperSetPosition(STEP_RIGHT_LIFT_ID, 0);
  stepperSetTarget(STEP_LEFT_LIFT_ID, STEP_LEFT_LIFT_RESET_OFFSET);
  stepperSetTarget(STEP_RIGHT_LIFT_ID, STEP_RIGHT_LIFT_RESET_OFFSET);
  stepperWait(STEP_LEFT_LIFT_ID);
  stepperWait(STEP_RIGHT_LIFT_ID);
  stepperSetPosition(STEP_LEFT_LIFT_ID, 0);
  stepperSetPosition(STEP_RIGHT_LIFT_ID, 0);
  stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
  stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);

  stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_RESET_OFFSET);
  stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_RESET_OFFSET);
  if(yellow)
  {
    stepperSetTarget(STEP_LEFT_LIFT_ID, 0);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -350);
  }
  else
  {
    stepperSetTarget(STEP_LEFT_LIFT_ID, STEP_LEFT_LIFT_INIT);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, STEP_RIGHT_LIFT_INIT);
  }
  stepperWait(STEP_LEFT_SLIDER_ID);
  stepperWait(STEP_RIGHT_SLIDER_ID);

  stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
  stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
  stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
  stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);

  stepperWait(STEP_LEFT_LIFT_ID);
  stepperWait(STEP_RIGHT_LIFT_ID);
  stepperWait(STEP_LEFT_SLIDER_ID);
  stepperWait(STEP_RIGHT_SLIDER_ID);

  //open clamps
  servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_INIT);
  servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_INIT);
  
  //open sliders
  servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
  servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
}

void handleStepActionPreTakeSpot(int right)
{
  if(right)
  {
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, -1900);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 0);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);

    //chThdSleepMilliseconds(1500);

    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);

  }
  else
  {
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, -100);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 2700);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);

    //chThdSleepMilliseconds(1500);

    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    //stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, -3000);
  }
}

void handleStepActionTakeSpot(int right)
{
  if(right)
  {
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_ALMOST_CLOSE);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, -1200);
    stepperWait(STEP_RIGHT_LIFT_ID);
    
    int i;
    for(i = 0; i < 10; i += 1)
    {
      servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_ALMOST_CLOSE - 50 * i);
      chThdSleepMilliseconds(75);
    }
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
    chThdSleepMilliseconds(250);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_ALMOST_CLOSE);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, -1400);
    stepperWait(STEP_RIGHT_LIFT_ID);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
    chThdSleepMilliseconds(250);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, -550);
  }
  else
  {
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE);

    stepperSetTarget(STEP_LEFT_LIFT_ID, 1200);
    stepperWait(STEP_LEFT_LIFT_ID);
    
    int i;
    for(i = 0; i < 10; i += 1)
    {
      servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE - 50 * i);
      chThdSleepMilliseconds(75);
    }
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    chThdSleepMilliseconds(250);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE);

    stepperSetTarget(STEP_LEFT_LIFT_ID, 1400);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    chThdSleepMilliseconds(250);

    stepperSetTarget(STEP_LEFT_LIFT_ID, 550);

    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);

    //servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE);

    //stepperSetTarget(STEP_LEFT_LIFT_ID, 1050);
    //stepperWait(STEP_LEFT_LIFT_ID);
    //servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    //chThdSleepMilliseconds(250);
    //servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE);

    //stepperSetTarget(STEP_LEFT_LIFT_ID, 1400);
    //stepperWait(STEP_LEFT_LIFT_ID);
    //servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    //chThdSleepMilliseconds(250);

    //stepperSetTarget(STEP_LEFT_LIFT_ID, 550);
  }
}

void handleStepActionLeaveSpot(int right)
{
  if(right)
  {
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_INIT);
    chThdSleepMilliseconds(500);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, 0);

  }
  else
  {
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_INIT);
    chThdSleepMilliseconds(500);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, 0);
  }
}

void handleStepActionLeaveSoftSpot(int right)
{
  if(right)
  {
    stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -1200);
    stepperWait(STEP_RIGHT_LIFT_ID);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_OPEN);

    chThdSleepMilliseconds(500);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, 1200);
  }
  else
  {
    stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperSetTarget(STEP_LEFT_LIFT_ID, 1200);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_OPEN);

    chThdSleepMilliseconds(500);

    stepperSetTarget(STEP_LEFT_LIFT_ID, -1200);
  }
}

void handleStepActionPrepSpot(int right)
{

    stepperWait(STEP_LEFT_LIFT_ID);
    stepperWait(STEP_RIGHT_LIFT_ID);
  if(right)
  {
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -550);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 0);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperWait(STEP_RIGHT_LIFT_ID);
  }
  else
  {
    stepperSetTarget(STEP_LEFT_LIFT_ID, 550);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    stepperSetTarget(STEP_LEFT_SLIDER_ID, 0);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperWait(STEP_LEFT_LIFT_ID);
    //stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
    //stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_RESET_OFFSET);
    //servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);

    //stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
    //stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_RESET_OFFSET);
    //servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    //stepperWait(STEP_LEFT_SLIDER_ID);
    //stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
    //stepperWait(STEP_RIGHT_SLIDER_ID);
    //stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
    //stepperSetTarget(STEP_RIGHT_SLIDER_ID, -350);
  }
}

void handleStepActionPrepCenter(void)
{
  servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
  servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

  stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
  stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);

  stepperWait(STEP_LEFT_SLIDER_ID);
  stepperWait(STEP_RIGHT_SLIDER_ID);
}

void handleStepActionPreTakeFirstBall(int right)
{
  if(right)
  {
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    chThdSleepMilliseconds(500);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 0);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, -1900);
    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);

  }
  else
  {
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    chThdSleepMilliseconds(500);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 2300);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, 0);
    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);

  }
}

void handleStepActionPreTakeFirst2Ball(int right)
{
  if(right)
  {
    stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
    stepperWait(STEP_LEFT_SLIDER_ID);

    stepperSetTarget(STEP_LEFT_SLIDER_ID, 2500);


    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_OPEN);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    chThdSleepMilliseconds(500);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 0);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, -1900);
    stepperWait(STEP_LEFT_SLIDER_ID);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    int i;
    for(i = SERVO_SLIDER_LEFT_CLOSE; i <= SERVO_SLIDER_LEFT_OPEN; i += (SERVO_SLIDER_LEFT_OPEN - SERVO_SLIDER_LEFT_CLOSE) / 10)
    {
    servoSet(SERVO_SLIDER_LEFT, i);
    chThdSleepMilliseconds(125);
    }
    //stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);

    //chThdSleepMilliseconds(500);
    //stepperSetTarget(STEP_LEFT_SLIDER_ID, 2500);

  }
  else
  {
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);
    stepperWait(STEP_RIGHT_SLIDER_ID);

    stepperSetTarget(STEP_RIGHT_SLIDER_ID, -2800);

    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_OPEN);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    chThdSleepMilliseconds(500);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 2300);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, 0);
    stepperWait(STEP_LEFT_SLIDER_ID);
    //stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);
  //  servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    //servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    int i;
    for(i = SERVO_SLIDER_RIGHT_CLOSE; i >= SERVO_SLIDER_RIGHT_OPEN; i += (SERVO_SLIDER_RIGHT_OPEN - SERVO_SLIDER_RIGHT_CLOSE) / 10)
    {
    servoSet(SERVO_SLIDER_RIGHT, i);
    chThdSleepMilliseconds(125);
    }
    //stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);


    //chThdSleepMilliseconds(500);
    //stepperSetTarget(STEP_RIGHT_SLIDER_ID, -2800);
  }
}

void handleStepActionTakeFirstBall(int right)
{
  if(right)
  {
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -1200);
    stepperWait(STEP_RIGHT_LIFT_ID);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
    chThdSleepMilliseconds(250);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, 0);
    stepperWait(STEP_RIGHT_LIFT_ID);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -550);

  }
  else
  {
    stepperSetTarget(STEP_LEFT_LIFT_ID, 1200);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    chThdSleepMilliseconds(250);


    stepperSetTarget(STEP_LEFT_LIFT_ID, 0);
    stepperWait(STEP_LEFT_LIFT_ID);
    stepperSetTarget(STEP_LEFT_LIFT_ID, 550);
  }
}

void handleStepActionTakeFirst2Ball(int right)
{
  if(right)
  {
    stepperSetTarget(STEP_LEFT_SLIDER_ID, 0);
    stepperWait(STEP_LEFT_SLIDER_ID);

    stepperSetTarget(STEP_LEFT_SLIDER_ID, 250);
  }
  else
  {
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 0);
    stepperWait(STEP_RIGHT_SLIDER_ID);

    stepperSetTarget(STEP_RIGHT_SLIDER_ID, -250);

  }

  (void)right;
  stepperWait(STEP_RIGHT_SLIDER_ID);
  stepperWait(STEP_LEFT_SLIDER_ID);

  if(right)
  {
    int i;
    for(i = SERVO_SLIDER_LEFT_OPEN; i >= 1400; i += (SERVO_SLIDER_LEFT_CLOSE - SERVO_SLIDER_LEFT_OPEN) / 8)
    {
    servoSet(SERVO_SLIDER_LEFT, i);
    chThdSleepMilliseconds(200);
    }
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
  }
  else
  {
    int i;
    for(i = SERVO_SLIDER_RIGHT_OPEN; i <= 1450; i += (SERVO_SLIDER_RIGHT_CLOSE - SERVO_SLIDER_RIGHT_OPEN) / 8)
    {
    servoSet(SERVO_SLIDER_RIGHT, i);
    chThdSleepMilliseconds(200);
    }
  }
  servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);


  stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
  stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);

  stepperSetTarget(STEP_RIGHT_LIFT_ID, -1200);
  stepperSetTarget(STEP_LEFT_LIFT_ID, 1200);
  stepperWait(STEP_LEFT_LIFT_ID);
  stepperWait(STEP_RIGHT_LIFT_ID);
  servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
  servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
  chThdSleepMilliseconds(250);

  stepperSetTarget(STEP_RIGHT_LIFT_ID, -550);
  stepperSetTarget(STEP_LEFT_LIFT_ID, 550);
}

void handleStepActionDisable(void)
{
  stepperDisable();

  servoSet(SERVO_SLIDER_LEFT, 0);
  servoSet(SERVO_SLIDER_RIGHT, 0);
  servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_OPEN);
  servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_OPEN);


}

int main(void)
{

  //RTOS initialization
  halInit();
  chSysInit();
  servoInit();
  stepperInit();
  stepInit();

  chThdCreateStatic(waHeartbeatThread, sizeof(waHeartbeatThread), NORMALPRIO, heartbeatThread, NULL);

  stepSetReady();


  while(1)
  {
    chThdSleepMilliseconds(10);
    step_action_t command = stepGetCommand();
    switch(command)
    {
      case STEP_ACTION_WAIT:
        continue;

      case STEP_ACTION_RESET_GREEN:
        handleStepActionReset(0);
        break;

      case STEP_ACTION_RESET_YELLOW:
        handleStepActionReset(1);
        break;


      case STEP_ACTION_TAKE_FIRST_BALL_LEFT:
        handleStepActionTakeFirstBall(0);
        break;

      case STEP_ACTION_PRETAKE_FIRST_BALL_LEFT:
        handleStepActionPreTakeFirstBall(0);
        break;

      case STEP_ACTION_TAKE_FIRST_2BALL_LEFT:
        handleStepActionTakeFirst2Ball(0);
        break;

      case STEP_ACTION_PRETAKE_FIRST_2BALL_LEFT:
        handleStepActionPreTakeFirst2Ball(0);
        break;



      case STEP_ACTION_DISABLE:
        handleStepActionDisable();
        break;

      case STEP_ACTION_TAKE_FIRST_BALL_RIGHT:
        handleStepActionTakeFirstBall(1);
        break;

      case STEP_ACTION_PRETAKE_FIRST_BALL_RIGHT:
        handleStepActionPreTakeFirstBall(1);
        break;

      case STEP_ACTION_TAKE_FIRST_2BALL_RIGHT:
        handleStepActionTakeFirst2Ball(1);
        break;

      case STEP_ACTION_PRETAKE_FIRST_2BALL_RIGHT:
        handleStepActionPreTakeFirst2Ball(1);
        break;



      case STEP_ACTION_PREP_SPOT_CENTER:
        handleStepActionPrepCenter();
        break;


      case STEP_ACTION_PREP_SPOT_LEFT:
        handleStepActionPrepSpot(0);
        break;

      case STEP_ACTION_PREP_SPOT_RIGHT:
        handleStepActionPrepSpot(1);
        break;

      case STEP_ACTION_PRETAKE_SPOT_LEFT:
        handleStepActionPreTakeSpot(0);
        break;

      case STEP_ACTION_TAKE_SPOT_LEFT:
        handleStepActionTakeSpot(0);
        break;

      case STEP_ACTION_LEAVE_SOFT_SPOT_LEFT:
        handleStepActionLeaveSoftSpot(0);
        break;

      case STEP_ACTION_LEAVE_SPOT_LEFT:
        handleStepActionLeaveSpot(0);
        break;

      case STEP_ACTION_PRETAKE_SPOT_RIGHT:
        handleStepActionPreTakeSpot(1);
        break;

      case STEP_ACTION_TAKE_SPOT_RIGHT:
        handleStepActionTakeSpot(1);
        break;

      case STEP_ACTION_LEAVE_SOFT_SPOT_RIGHT:
        handleStepActionLeaveSoftSpot(1);
        break;

      case STEP_ACTION_LEAVE_SPOT_RIGHT:
        handleStepActionLeaveSpot(1);
        break;

      case STEP_ACTION_TAKE_RIGHT:
        handleStepActionTakeRight();
        break;

      default:
        break;
    }
    stepSetReady();
  }

  return 0;
}
