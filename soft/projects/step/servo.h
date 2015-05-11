#ifndef SERVO_H
#define SERVO_H

typedef enum 
{
  SERVO_CLAMP_LEFT,
  SERVO_CLAMP_RIGHT,
  SERVO_SLIDER_LEFT,
  SERVO_SLIDER_RIGHT,
} servo_t;

void servoInit(void);
void servoSet(servo_t id, uint16_t value);

#endif// SERVO_H
