#ifndef PID_H
#define PID_H

#include <ch.h>

typedef struct {
  double kp;
  double kd;
  double ki;
  double imax;

  double set_point;
  double feedback;
  double integral;
  double error;
  double output;

  Mutex mutex;
} pid_t;

void pidInit(pid_t * pid);

void pidCompute(pid_t * pid);

void pidSetSetPoint(pid_t * pid, double set_point);
void pidSetFeedback(pid_t * pid, double feedback);
void pidSetKp(pid_t * pid, double kp);
void pidSetKd(pid_t * pid, double kd);
void pidSetKi(pid_t * pid, double ki);
void pidSetImax(pid_t * pid, double imax);
double pidGetOutput(pid_t * pid);

#endif// PID_H
