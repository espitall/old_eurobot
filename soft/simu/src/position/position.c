#include "position.h"
#include "../config.h"

double posGetXmm(void)
{
  return 70 + ROBOT_X / 2;
}

double posGetYmm(void)
{
  return 1000;
}

/*
 * Angle en degrés
 */
double posGetAdeg(void)
{
  return 90;
}