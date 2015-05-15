#ifndef RAMP_H
#define RAMP_H

#include <ch.h>

typedef struct {
  double max_accel;
  double max_speed;

  double set_point;
  double speed_set_point;
  double output;

  Mutex mutex;
} ramp_t;

void rampInit(ramp_t * ramp);

void rampCompute(ramp_t * ramp);

void rampSetMaxAccel(ramp_t * ramp, double max_accel);

void rampSetMaxSpeed(ramp_t * ramp, double max_speed);

void rampSetSetPoint(ramp_t * ramp, double set_point);

double rampGetOutput(ramp_t * ramp);

void rampReset(ramp_t * ramp, double preset);

void rampSetSetPointNow(ramp_t * ramp, double set_point);

#endif// RAMP_H
