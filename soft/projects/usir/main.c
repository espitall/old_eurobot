#include <ch.h>
#include <hal.h>
#include <usir.h>


static WORKING_AREA(waHeartbeatThread, 256);

msg_t heartbeatThread(void* arg) 
{
  (void)arg;

  while (1)
  {
    palSetPad(GPIOD, GPIOD_LED);
    chThdSleepMilliseconds(500);

    palClearPad(GPIOD, GPIOD_LED);
    chThdSleepMilliseconds(500);
  }

  return 0;
}

ISR(ADCA_CH0_vect)
{
  usirSetIRRaw(0, ADCA.CH0.RES);
}

ISR(ADCA_CH1_vect)
{
  usirSetIRRaw(1, ADCA.CH1.RES);
}

ISR(ADCA_CH2_vect)
{
  usirSetIRRaw(2, ADCA.CH2.RES);
}

ISR(ADCA_CH3_vect)
{
  usirSetIRRaw(3, ADCA.CH3.RES);
}

void adcInit(void)
{
  ADCA.CTRLA = ADC_ENABLE_bm;
  ADCA.CTRLB = ADC_FREERUN_bm;
  ADCA.EVCTRL = ADC_EVACT_CH0123_gc;
  ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;

  ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
  ADCA.CH0.CTRL = ADC_CH_START_bm | ADC_CH_INPUTMODE_SINGLEENDED_gc;

  ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
  ADCA.CH1.CTRL = ADC_CH_START_bm | ADC_CH_INPUTMODE_SINGLEENDED_gc;

  ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
  ADCA.CH2.CTRL = ADC_CH_START_bm | ADC_CH_INPUTMODE_SINGLEENDED_gc;

  ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
  ADCA.CH3.CTRL = ADC_CH_START_bm | ADC_CH_INPUTMODE_SINGLEENDED_gc;

  ADCA.CH0.INTCTRL = ADC_CH_INTLVL_MED_gc;
  ADCA.CH1.INTCTRL = ADC_CH_INTLVL_MED_gc;
  ADCA.CH3.INTCTRL = ADC_CH_INTLVL_MED_gc;
  ADCA.CH2.INTCTRL = ADC_CH_INTLVL_MED_gc;
}

int main(void)
{
  //RTOS initialization
  halInit();
  chSysInit();
  usirInit();

  adcInit();

  chThdCreateStatic(waHeartbeatThread, sizeof(waHeartbeatThread), NORMALPRIO, heartbeatThread, NULL);

  while(1)
  {
    chThdSleepMilliseconds(100);
  }

  return 0;
}
