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

ISR(TCE0_CCA_vect)
{
  usirSetUSRaw(0, TCE0.CCA);
}

ISR(TCD0_CCA_vect)
{
  usirSetUSRaw(1, TCD0.CCA);
}

ISR(TCC0_CCA_vect)
{
  usirSetUSRaw(2, TCC0.CCA);
}

ISR(TCC1_CCA_vect)
{
  usirSetUSRaw(3, TCC1.CCA);
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
  //ADCA.CTRLB = 0;//ADC_FREERUN_bm;
  //ADCA.EVCTRL = ADC_SWEEP_0123_gc;
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

  PORTB.DIRSET = (1 << 0);
  PORTB.OUTSET = (1 << 0);

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

  TCD0.CTRLD = TC_EVACT_PW_gc | TC_EVSEL_CH1_gc;
  TCD0.CTRLB = TC0_CCAEN_bm;
  TCD0.CTRLA = TC_CLKSEL_DIV8_gc;
  TCD0.INTCTRLB = TC_CCAINTLVL_LO_gc;
  TCD0.PER = 0xFFFF;

  TCC0.CTRLD = TC_EVACT_PW_gc | TC_EVSEL_CH2_gc;
  TCC0.CTRLB = TC0_CCAEN_bm;
  TCC0.CTRLA = TC_CLKSEL_DIV8_gc;
  TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
  TCC0.PER = 0xFFFF;

  TCC1.CTRLD = TC_EVACT_PW_gc | TC_EVSEL_CH3_gc;
  TCC1.CTRLB = TC0_CCAEN_bm;
  TCC1.CTRLA = TC_CLKSEL_DIV8_gc;
  TCC1.INTCTRLB = TC_CCAINTLVL_LO_gc;
  TCC1.PER = 0xFFFF;

  volatile uint32_t i;
  while(1)
  {
    palSetPad(GPIOC, GPIOC_US_TRIG_CH0);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH0);
    ADCA.CH0.CTRL |= ADC_CH_START_bm;
    chThdSleepMilliseconds(20);

    palSetPad(GPIOC, GPIOC_US_TRIG_CH1);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH1);
    ADCA.CH1.CTRL |= ADC_CH_START_bm;
    chThdSleepMilliseconds(20);

    palSetPad(GPIOC, GPIOC_US_TRIG_CH2);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH2);
    ADCA.CH2.CTRL |= ADC_CH_START_bm;
    chThdSleepMilliseconds(20);

    palSetPad(GPIOC, GPIOC_US_TRIG_CH3);
    for(i = 0; i < 10; i += 1);
    palClearPad(GPIOC, GPIOC_US_TRIG_CH3);
    ADCA.CH3.CTRL |= ADC_CH_START_bm;
    chThdSleepMilliseconds(20);

    uint8_t stop = 0;
    int32_t alert = getAlert();
    if(alert > 0)
    {
      uint8_t ids = alert >> 12;
      alert = alert & 0xFFF;
      for(i = 0; i < 4; i += 1)
      {
        if(ids & (1 << i))
        {
          if(usirGetUSRaw(i) < alert)
          {
            if(usirGetIRRaw(i) < alert * 1.30)
            {
              stop = 1;
            }
          }
        }
      }
    }

    if(stop)
    {
      PORTB.OUTSET = (1 << 0);
    }
    else
    {
      PORTB.OUTCLR = (1 << 0);
    }
  }

  return 0;
}
