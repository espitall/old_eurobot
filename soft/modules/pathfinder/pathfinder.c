#include <trajectory.h>
#include "pathfinder.h"

void pathfinderInit(void)
{
}

void pathfinderGotoXYmm(double x, double y)
{
  TRAJECTORY_XY_MM(10, 10);
  TRAJECTORY_XY_MM(252, 752);
  TRAJECTORY_XY_MM(x, y);
}
