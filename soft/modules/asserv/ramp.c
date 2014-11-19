#include "ramp.h"

void rampInit(ramp_t * ramp)
{
  chMtxInit(&ramp->mutex);

  ramp->max_accel = 0;
  ramp->max_speed = 0;

  ramp->set_point = 0;
  ramp->speed_set_point = 0;
  ramp->output = 0;
}

void rampSetMaxAccel(ramp_t * ramp, double max_accel)
{
  chMtxLock(&ramp->mutex);
  ramp->max_accel = max_accel;
  chMtxUnlock();
}

void rampSetMaxSpeed(ramp_t * ramp, double max_speed)
{
  chMtxLock(&ramp->mutex);
  ramp->max_speed = max_speed;
  chMtxUnlock();
}

void rampSetSetPoint(ramp_t * ramp, double set_point)
{
  chMtxLock(&ramp->mutex);
  ramp->set_point = set_point;
  chMtxUnlock();
}

double rampGetOutput(ramp_t * ramp)
{
  double output;

  chMtxLock(&ramp->mutex);
  output = ramp->output;
  chMtxUnlock();

  return output;
}

void rampCompute(ramp_t * ramp)
{
  chMtxLock(&ramp->mutex);
  if(ramp->speed_set_point > 0)
  {
    if((ramp->speed_set_point * ramp->speed_set_point) / (2 *  ramp->max_accel) + ramp->output < ramp->set_point)
    {
      if(ramp->speed_set_point < ramp->max_speed)
      {
        ramp->speed_set_point += ramp->max_accel;
      }
    }
    else
    {
      if(ramp->speed_set_point > - ramp->max_speed)
      {
        ramp->speed_set_point -= ramp->max_accel;
      }
    }
  }
  else
  {
    if(-(ramp->speed_set_point * ramp->speed_set_point) / (2 *  ramp->max_accel) + ramp->output < ramp->set_point)
    {
      if(ramp->speed_set_point < ramp->max_speed)
      {
        ramp->speed_set_point += ramp->max_accel;
      }
    }
    else
    {
      if(ramp->speed_set_point > -ramp->max_speed)
      {
        ramp->speed_set_point -= ramp->max_accel;
      }
    }
  }
  ramp->output += ramp->speed_set_point;
  chMtxUnlock();
}
