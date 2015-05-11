#include <position.h>
#include <dc_motors.h>
#include <strat.h>
#include "asserv.h"
#include "ramp.h"
#include "pid.h"
#include "config.h"

static ramp_t _dist_ramp;
static ramp_t _angu_ramp;
static pid_t _dist_pid;
static pid_t _angu_pid;
static volatile int enabled;

void asservSetEnable(int a)
{
  if(enabled == 0)
  {
    rampReset(&_dist_ramp, posGetDmm());
    rampReset(&_angu_ramp, posGetAdeg());

    pidReset(&_dist_pid);
    pidReset(&_angu_pid);
  }

  enabled = a;
}

int asservIsEnabled(void)
{
  return enabled;
}


void asservSlow(void)
{
  rampSetMaxAccel(&_angu_ramp, ASSERV_ANGU_MAX_ACCEL / 5);
}

void asservInit(void)
{
  enabled = 0;

  rampInit(&_dist_ramp);
  rampInit(&_angu_ramp);

  pidInit(&_dist_pid);
  pidInit(&_angu_pid);

  rampSetMaxAccel(&_dist_ramp, ASSERV_DIST_MAX_ACCEL);
  rampSetMaxAccel(&_angu_ramp, ASSERV_ANGU_MAX_ACCEL);

  rampSetMaxSpeed(&_dist_ramp, ASSERV_DIST_MAX_SPEED);
  rampSetMaxSpeed(&_angu_ramp, ASSERV_ANGU_MAX_SPEED);

  pidSetKp(&_dist_pid, ASSERV_DIST_KP);
  pidSetKd(&_dist_pid, ASSERV_DIST_KD);
  pidSetKi(&_dist_pid, ASSERV_DIST_KI);
  pidSetImax(&_dist_pid, ASSERV_DIST_IMAX);

  pidSetKp(&_angu_pid, ASSERV_ANGU_KP);
  pidSetKd(&_angu_pid, ASSERV_ANGU_KD);
  pidSetKi(&_angu_pid, ASSERV_ANGU_KI);
  pidSetImax(&_angu_pid, ASSERV_ANGU_IMAX);
}

void asservCompute(void)
{
  if(enabled)
  {
    if(stratGetTimeLeft() <= 0)
    {
      asservSetEnable(0);
    }
  }

  pidSetFeedback(&_dist_pid, posGetDmm());
  pidSetFeedback(&_angu_pid, posGetAdeg());

  rampCompute(&_dist_ramp);
  rampCompute(&_angu_ramp);

  pidSetSetPoint(&_dist_pid, rampGetOutput(&_dist_ramp));
  pidSetSetPoint(&_angu_pid, rampGetOutput(&_angu_ramp));
  //pidSetSetPoint(&_dist_pid, 0);
  //pidSetSetPoint(&_angu_pid, 0);
  

  pidCompute(&_dist_pid);
  pidCompute(&_angu_pid);

  double d_out = pidGetOutput(&_dist_pid);
  double a_out = pidGetOutput(&_angu_pid);

  if(enabled)
  {
    double moteur0 = d_out + a_out;
    double moteur1 = d_out - a_out;
    
    if (moteur0 < -10000)
    {
      moteur0 = -10000;
    }
    else if(moteur0 > 10000)
    {
      moteur0 = 10000;
    }
    
    if (moteur1 < -10000)
    {
      moteur1 = -10000;
    }
    else if(moteur1 > 10000)
    {
      moteur1 = 10000;
    }
    
    dcmSetWidth(0, moteur0);
    dcmSetWidth(1, moteur1);
  }
  else
  {
    rampReset(&_dist_ramp, posGetDmm());
    rampReset(&_angu_ramp, posGetAdeg());

    pidReset(&_dist_pid);
    pidReset(&_angu_pid);

    dcmSetWidth(0, 0);
    dcmSetWidth(1, 0);
  }

}

void asservSetDistanceSetPoint(double set_point)
{
  rampSetSetPoint(&_dist_ramp,set_point);
}

void asservSetAngularSetPoint(double set_point)
{
  rampSetSetPoint(&_angu_ramp,set_point);
}
