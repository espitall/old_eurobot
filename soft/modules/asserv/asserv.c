#include <position.h>
#include <dc_motors.h>
#include "asserv.h"
#include "ramp.h"
#include "pid.h"
#include "config.h"

static ramp_t _dist_ramp;
static ramp_t _angu_ramp;
static pid_t _dist_pid;
static pid_t _angu_pid;
static volatile int enabled;


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
}

void asservCompute(void)
{
  pidSetFeedback(&_dist_pid, posGetDmm());
  pidSetFeedback(&_angu_pid, posGetAdeg());

  rampCompute(&_dist_ramp);
  rampCompute(&_angu_ramp);

  pidSetSetPoint(&_dist_pid, rampGetOutput(&_dist_ramp));
  pidSetSetPoint(&_angu_pid, rampGetOutput(&_angu_ramp));

  pidCompute(&_dist_pid);
  pidCompute(&_angu_pid);

  double d_out = pidGetOutput(&_dist_pid);
  double a_out = pidGetOutput(&_angu_pid);

  if(enabled)
  {
    dcmSetWidth(0, d_out + a_out);
    dcmSetWidth(1, d_out - a_out);
  }
  else
  {
    rampReset(&_dist_ramp);
    rampReset(&_angu_ramp);

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
