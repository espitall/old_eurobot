#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#define TRAJECTORY_MAX_ORDER 15

#define TRAJECTORY_FLAGS_T_REL (1 << 0)

typedef enum
{
  TRAJECTORY_TYPE_NO_MOVE,
  TRAJECTORY_TYPE_D,
  TRAJECTORY_TYPE_A,
  TRAJECTORY_TYPE_T,
  TRAJECTORY_TYPE_XY,
  TRAJECTORY_TYPE_WEDGING,
} trajectoryType_t;

void trajectoryInit(void);
void trajectoryWait(void);
void trajectoryPrint(void);
void trajevtorySetSafetymm(double d);

void _trajectoryNewOrder(trajectoryType_t type, double d, double a, double x, double y, double t, unsigned int flags);


#define TRAJECTORY_TEMP_S(tempo) _trajectoryNewOrder(TRAJECTORY_TYPE_T, 0, 0, 0, 0, tempo, TRAJECTORY_FLAGS_T_REL)
#define TRAJECTORY_D_MM(dist) _trajectoryNewOrder(TRAJECTORY_TYPE_D, dist, 0, 0, 0, 0, 0)
#define TRAJECTORY_A_DEG(angu) _trajectoryNewOrder(TRAJECTORY_TYPE_A, 0, angu, 0, 0, 0, 0)
#define TRAJECTORY_XY_MM(x, y) _trajectoryNewOrder(TRAJECTORY_TYPE_XY, 0, 0, x, y, 0, 0)
#define TRAJECTORY_WEDGING() _trajectoryNewOrder(TRAJECTORY_TYPE_WEDGING, 0, 0, 0, 0, 0, 0);

#endif //TRAJECTORY_H
