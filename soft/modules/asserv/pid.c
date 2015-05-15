#include "pid.h"

void pidInit(pid_t * pid)
{
  chMtxInit(&pid->mutex);

  pid->kp = 0;
  pid->kd = 0;
  pid->ki = 0;
  pid->imax = 0;

  pid->set_point = 0;
  pid->feedback = 0;
  pid->integral = 0;
  pid->output = 0;
  pid->error = 0;
}

void pidSetSetPoint(pid_t * pid, double set_point)
{
  chMtxLock(&pid->mutex);
  pid->set_point = set_point;
  chMtxUnlock();
}

void pidSetFeedback(pid_t * pid, double feedback)
{
  chMtxLock(&pid->mutex);
  pid->feedback = feedback;
  chMtxUnlock();
}

void pidSetKp(pid_t *pid, double kp)
{
  chMtxLock(&pid->mutex);
  pid->kp = kp;
  chMtxUnlock();
}

void pidSetKd(pid_t *pid, double kd)
{
  chMtxLock(&pid->mutex);
  pid->kd = kd;
  chMtxUnlock();
}

void pidSetKi(pid_t *pid, double ki)
{
  chMtxLock(&pid->mutex);
  pid->ki = ki;
  chMtxUnlock();
}

void pidSetImax(pid_t *pid, double imax)
{
  chMtxLock(&pid->mutex);
  pid->imax = imax;
  chMtxUnlock();
}

double pidGetOutput(pid_t * pid)
{
  double output;
  chMtxLock(&pid->mutex);
  output = pid->output;
  chMtxUnlock();

  return output;
}

void pidCompute(pid_t * pid)
{
  chMtxLock(&pid->mutex);

  double error = pid->set_point - pid->feedback;

  pid->integral += error;

  if(pid->integral > pid->imax)
  {
    pid->integral = pid->imax;
  }
  else if(pid->integral < -pid->imax)
  {
    pid->integral = -pid->imax;
  }

  double derivate = error - pid->error;
  pid->error = error;

  pid->output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivate;

  chMtxUnlock();
}

void pidReset(pid_t * pid)
{
  chMtxLock(&pid->mutex);

  pid->integral = 0;

  chMtxUnlock();
}

