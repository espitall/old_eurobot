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
#include <gyro.h>
#include <trajectory.h>

void position_computed_hook(void)
{
  asservCompute();
}

int main(void) 
{
  //RTOS initialization
  halInit();
  chSysInit();

  palSetPad(GPIOG, GPIOG_LED4_RED);

  //modules and peripherals initialization
  pcm9685Init();
  dcmInit();
  lcdInit();
  fieldInit();
  posInit(position_computed_hook);
  trajectoryInit();
  asservInit();
  max11628Init();
  //gyroInit();

  lcdPrintln("Start: robot principal");

  
  lcdPrintln("Asserv: attente 3s");
  chThdSleepMilliseconds(3000);
  asservSetEnable(1);
  lcdPrintln("Asserv: ok");

  //test du module trajectoire (sans mouvement)


  //dcmSetWidth (0, 800);
  //dcmSetWidth (1, -800);

  //int32_t distance;
  //int32_t angle;
  
  //int i = 20;
  //int j = 10;
  //while(true)
  //{
  //  TRAJECTORY_A_DEG(-i);
  //  trajectoryWait();

  //  TRAJECTORY_A_DEG(j);
  //  trajectoryWait();

  //  i += 10;
  //  j += 5;
  //}

  TRAJECTORY_D_MM(700);
  TRAJECTORY_A_DEG(20);
  TRAJECTORY_A_DEG(-20);
  TRAJECTORY_A_DEG(180);
  TRAJECTORY_D_MM(700);
  trajectoryWait();

  while (true)
  {
    chThdSleepMilliseconds(10);
    /*
    dcmSetWidth(0,i);
    i += 10;*/
    /*
    distance = (int) posGetDmm();
    lcdPrintln("%ld mm", distance);
    */
    /*
    angle = (int) posGetAdeg();
    lcdPrintln("%ld deg", angle);
    */
  }
}
