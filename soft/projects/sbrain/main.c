#include <ch.h>
#include <hal.h>
#include <lcd.h>
#include <field.h>
#include <dc_motors.h>


int main(void) 
{
  //RTOS initialization
  halInit();
  chSysInit();

  palSetPad(GPIOG, GPIOG_LED4_RED);

  //modules and peripherals initialization
  dcmInit();
  lcdInit();
  fieldInit();


  while (TRUE) {

    uint16_t raw = 0x0000;
    palClearPad(GPIOE, GPIOE_SPI4_ENC0_CS);
    spiReceive(&SPID4, 2, &raw);
    palSetPad(GPIOE, GPIOE_SPI4_ENC0_CS);
    raw = ((raw & 0xff) << 8) | (raw >> 8);
    raw <<= 1;

    lcdPrintln("raw: 0x%04X (%u)", raw, raw);
    chThdSleepMilliseconds(500);
  }

  while(TRUE);
}
