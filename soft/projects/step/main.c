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

int main(void)
{

  //RTOS initialization
  halInit();
  chSysInit();
  servoInit();
  stepperInit();
  stepInit();

  chThdCreateStatic(waHeartbeatThread, sizeof(waHeartbeatThread), NORMALPRIO, heartbeatThread, NULL);

  initMeca();
  stepSetReady();


  while(1)
  {
    chThdSleepMilliseconds(10);
    step_action_t command = stepGetCommand();
    switch(command)
    {
      case STEP_ACTION_WAIT:
        continue;

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
