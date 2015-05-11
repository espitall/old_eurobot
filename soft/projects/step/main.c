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

void handleStepActionReset(void)
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
  stepperSetTarget(STEP_LEFT_LIFT_ID, STEP_LEFT_LIFT_INIT);
  stepperSetTarget(STEP_RIGHT_LIFT_ID, STEP_RIGHT_LIFT_INIT);
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
    stepperSetRel(STEP_LEFT_SLIDER_ID, -3000);
    stepperSetRel(STEP_RIGHT_SLIDER_ID, -3000);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperSetRel(STEP_LEFT_SLIDER_ID, 150);
    stepperSetRel(STEP_RIGHT_SLIDER_ID, 150);
    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperSetRel(STEP_RIGHT_SLIDER_ID, -100);

    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    stepperSetRel(STEP_LEFT_SLIDER_ID, 350);


  }
  else
  {
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);
    stepperSetRel(STEP_LEFT_SLIDER_ID, 3000);
    stepperSetRel(STEP_RIGHT_SLIDER_ID, 3000);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperSetRel(STEP_LEFT_SLIDER_ID, -250);
    stepperSetRel(STEP_RIGHT_SLIDER_ID, -250);
    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperSetRel(STEP_LEFT_SLIDER_ID, 100);

    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    stepperSetRel(STEP_RIGHT_SLIDER_ID, -350);
  }
}

void handleStepActionTakeSpot(int right)
{
  if(right)
  {
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_ALMOST_CLOSE);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, -1200);
    stepperWait(STEP_RIGHT_LIFT_ID);
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

    stepperSetTarget(STEP_LEFT_LIFT_ID, 1050);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    chThdSleepMilliseconds(250);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE);

    stepperSetTarget(STEP_LEFT_LIFT_ID, 1400);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    chThdSleepMilliseconds(250);

    stepperSetTarget(STEP_LEFT_LIFT_ID, 550);
  }
}

void handleStepActionLeaveSpot(int right)
{
  if(right)
  {

  }
  else
  {
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_INIT);
    chThdSleepMilliseconds(500);
  }
}

void handleStepActionPrepSpot(int right)
{
  if(right)
  {
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 0);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperWait(STEP_RIGHT_SLIDER_ID);
  }
  else
  {
    stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
    stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_RESET_OFFSET);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);

    stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_RESET_OFFSET);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);

    stepperWait(STEP_LEFT_SLIDER_ID);
    stepperSetPosition(STEP_LEFT_SLIDER_ID, 0);
    stepperWait(STEP_RIGHT_SLIDER_ID);
    stepperSetPosition(STEP_RIGHT_SLIDER_ID, 0);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, -350);
  }
}

void handleStepActionTakeFirstBall(int right)
{
  (void)right;

  servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_OPEN);
  stepperSetTarget(STEP_LEFT_SLIDER_ID, STEP_LEFT_SLIDER_INIT);
  stepperSetTarget(STEP_RIGHT_SLIDER_ID, STEP_RIGHT_SLIDER_INIT);
  stepperWait(STEP_LEFT_SLIDER_ID);
  stepperWait(STEP_RIGHT_SLIDER_ID);

  servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_CLOSE);
  servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_CLOSE);

  if(right)
  {
    stepperSetTarget(STEP_LEFT_SLIDER_ID, 4500);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, -500);
  }
  else
  {
    stepperSetTarget(STEP_LEFT_SLIDER_ID, -500);
    stepperSetTarget(STEP_RIGHT_SLIDER_ID, 2000);

  }
  stepperWait(STEP_LEFT_SLIDER_ID);
  stepperWait(STEP_RIGHT_SLIDER_ID);

  stepperWait(STEP_LEFT_SLIDER_ID);
  stepperWait(STEP_RIGHT_SLIDER_ID);

  
  if(right)
  {
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -830);
    stepperWait(STEP_RIGHT_LIFT_ID);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    chThdSleepMilliseconds(250);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_ALMOST_CLOSE);
    stepperSetTarget(STEP_RIGHT_LIFT_ID, -1200);
    stepperWait(STEP_RIGHT_LIFT_ID);
    servoSet(SERVO_CLAMP_RIGHT, SERVO_CLAMP_RIGHT_CLOSE);
    chThdSleepMilliseconds(250);

    stepperSetTarget(STEP_RIGHT_LIFT_ID, -300);
    stepperWait(STEP_RIGHT_LIFT_ID);

  }
  else
  {
    stepperSetTarget(STEP_LEFT_LIFT_ID, 830);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    servoSet(SERVO_SLIDER_LEFT, SERVO_SLIDER_LEFT_OPEN);
    servoSet(SERVO_SLIDER_RIGHT, SERVO_SLIDER_RIGHT_OPEN);
    chThdSleepMilliseconds(250);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_ALMOST_CLOSE);
    stepperSetTarget(STEP_LEFT_LIFT_ID, 1100);
    stepperWait(STEP_LEFT_LIFT_ID);
    servoSet(SERVO_CLAMP_LEFT, SERVO_CLAMP_LEFT_CLOSE);
    chThdSleepMilliseconds(250);

    stepperSetTarget(STEP_LEFT_LIFT_ID, 400);
    stepperWait(STEP_LEFT_LIFT_ID);
  }
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

      case STEP_ACTION_RESET:
        handleStepActionReset();
        break;

      case STEP_ACTION_TAKE_FIRST_BALL_LEFT:
        handleStepActionTakeFirstBall(0);
        break;

      case STEP_ACTION_TAKE_FIRST_BALL_RIGHT:
        handleStepActionTakeFirstBall(1);
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

      case STEP_ACTION_LEAVE_SPOT_LEFT:
        handleStepActionLeaveSpot(0);
        break;

      case STEP_ACTION_PRETAKE_SPOT_RIGHT:
        handleStepActionPreTakeSpot(1);
        break;

      case STEP_ACTION_TAKE_SPOT_RIGHT:
        handleStepActionTakeSpot(1);
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
