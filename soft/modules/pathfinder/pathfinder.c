#include <trajectory.h>
#include "pathfinder.h"

void pathfinderInit(void)
{
}

void pathfinderGotoXYmm(double x, double y)
{
  // position du robot
  double bot_x = posGetXmm();
  double bot_y = posGetYmm();
  
  // Taille du robot : ROBOT_X et ROBOT_Y
  TRAJECTORY_XY_MM(10, 10);
  TRAJECTORY_XY_MM(252, 752);
  TRAJECTORY_XY_MM(x, y);
}
