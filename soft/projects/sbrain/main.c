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
#include <usir.h>
#include <gyro.h>

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
  asservInit();
  max11628Init();
  usirInit();
  //gyroInit();

  lcdPrintln("Start: robot secondaire");

  while (true)
  {
    pcm9685SetChannel(0, 0, 300);
    chThdSleepMilliseconds(5000);

    pcm9685SetChannel(0, 0, 340);
    chThdSleepMilliseconds(5000);
  }
}
