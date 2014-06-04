#include "xmega/clock/clock.h"
#include "pwm.h"


void pwm_init(pwm_t * s,TC0_t * timer,TC_CLKSEL_t pres, uint8_t output)
{
  s->t = timer;

  s->sign_port[0] = (PORT_t *)0;
  s->sign_port[1] = (PORT_t *)0;
  s->sign_port[2] = (PORT_t *)0;
  s->sign_port[3] = (PORT_t *)0;

  timer->CTRLA = pres;
  timer->CTRLB = output | TC_WGMODE_SS_gc;
  timer->PER = 0;
  timer->CCA = 0;
  timer->CCB = 0;
  timer->CCC = 0;
  timer->CCD = 0;
}

void pwm_set_sign(pwm_t *s, uint8_t output, PORT_t * p, uint8_t bit){
  if(output >= 'A' && output <= 'D')
  {
    output -= 'A';
  }
  if(output <= 4)
  {
    s->sign_bit[output] = 1 << bit;
    s->sign_port[output] = p;
  }
}

void pwm_set_period(pwm_t * s, uint16_t per)
{
  s->t->PER = per;
}

void pwm_set_output(pwm_t * s, uint8_t output,int16_t value)
{
  if(output >= 'A' && output <= 'D')
  {
    output -= 'A';
  }
  if(s->sign_port[output] != (PORT_t *)0)
  {
    if(value >= 0)
    {
      s->sign_port[output]->OUTSET = s->sign_bit[output]; 
    }
    else
    {
      s->sign_port[output]->OUTCLR = s->sign_bit[output]; 
    }
  }
  if(value < 0)
  {
      value = -value;
  }
  
  switch(output)
  {
    case 0:
      s->t->CCA = value;
      break;

    case 1:
      s->t->CCB = value;
      break;

    case 2:
      s->t->CCC = value;
      break;

    case 3:
      s->t->CCD = value;
      break;
  }
}

uint32_t pwm_get_frequency(pwm_t * s)
{
#ifndef SIMULATION 
  uint32_t clk = 0;
  switch(s->t->CTRLA & TC0_CLKSEL_gm)
  {
    case TC_CLKSEL_OFF_gc:
      clk = 0;
      break;
    case TC_CLKSEL_DIV1_gc:
      clk = CLOCK_CPU_FREQ;
      break;
    case TC_CLKSEL_DIV2_gc:
      clk = CLOCK_CPU_FREQ/2;
      break;
    case TC_CLKSEL_DIV4_gc:
      clk = CLOCK_CPU_FREQ/4;
      break;
    case TC_CLKSEL_DIV8_gc:
      clk = CLOCK_CPU_FREQ/8;
      break;
    case TC_CLKSEL_DIV64_gc:
      clk = CLOCK_CPU_FREQ/64;
      break;
    case TC_CLKSEL_DIV256_gc:
      clk = CLOCK_CPU_FREQ/256;
      break;
    case TC_CLKSEL_DIV1024_gc:
      clk = CLOCK_CPU_FREQ/1024;
      break;
  }
  return clk/s->t->PER;
#else
  return 1;
#endif
}

