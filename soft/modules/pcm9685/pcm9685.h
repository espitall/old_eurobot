#ifndef PWM_H
#define PWM_H

void pcm9685Init(void);
void pcm9685SetChannel(uint8_t id, uint16_t delay, uint16_t width);

#endif// PWM_H
