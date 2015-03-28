#include <ch.h>
#include <hal.h>
#include <usir.h>


static WORKING_AREA(waHeartbeatThread, 256);
static WORKING_AREA(waUSThread, 256);

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

ISR(TCE0_CCA_vect)
{
  usirSetUSRaw(0, TCE0.CCA);
}

msg_t usThread(void* arg) 
{
  (void)arg;

  //init event system
  EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN7_gc;
  EVSYS.CH1MUX = EVSYS_CHMUX_PORTC_PIN6_gc;
  EVSYS.CH2MUX = EVSYS_CHMUX_PORTC_PIN3_gc;
  EVSYS.CH3MUX = EVSYS_CHMUX_PORTC_PIN2_gc;

  //init timer capture
  TCE0.CTRLD = TC_EVACT_PW_gc | TC_EVSEL_CH0_gc;
  TCE0.CTRLB = TC0_CCAEN_bm;
  TCE0.CTRLA = TC_CLKSEL_DIV8_gc;
  TCE0.INTCTRLB = TC_CCAINTLVL_LO_gc;
  TCE0.PER = 0xFFFF;


  volatile uint32_t i;
  while(1)
  {
    palSetPad(GPIOC, GPIOC_US_TRIG_CH0);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH0);
    chThdSleepMilliseconds(20);

    palSetPad(GPIOC, GPIOC_US_TRIG_CH1);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH1);
    chThdSleepMilliseconds(20);

    palSetPad(GPIOC, GPIOC_US_TRIG_CH2);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH2);
    chThdSleepMilliseconds(20);

    palSetPad(GPIOC, GPIOC_US_TRIG_CH3);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH3);
    chThdSleepMilliseconds(20);
  }
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
  ADCA.EVCTRL = ADC_SWEEP_0123_gc;
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
  ADCA.CH2.INTCTRL = ADC_CH_INTLVL_MED_gc;
  ADCA.CH3.INTCTRL = ADC_CH_INTLVL_MED_gc;
}

int main(void)
{
  //RTOS initialization
  halInit();
  chSysInit();
  usirInit();
  adcInit();

  chThdCreateStatic(waHeartbeatThread, sizeof(waHeartbeatThread), NORMALPRIO, heartbeatThread, NULL);
  chThdCreateStatic(waUSThread, sizeof(waUSThread), NORMALPRIO, usThread, NULL);

  while(1)
  {
    chThdSleepMilliseconds(100);
  }

  return 0;
}
