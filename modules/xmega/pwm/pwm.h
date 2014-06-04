#ifndef PWM_H
#define PWM_H
#include <avr/io.h>


typedef struct pwm_xmega_xmega_s
{
  TC0_t *t;
  PORT_t * sign_port[4];
  uint8_t sign_bit[4];
}pwm_t;

void pwm_init(pwm_t * s,TC0_t * timer,TC_CLKSEL_t pres, uint8_t output);
void pwm_set_sign(pwm_t *s, uint8_t output, PORT_t * p, uint8_t bit);
void pwm_set_period(pwm_t * s, uint16_t per);
void pwm_set_output(pwm_t * s, uint8_t output,int16_t value);
uint32_t pwm_get_frequency(pwm_t * s);

#endif //PWM_H
