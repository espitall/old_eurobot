#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#define TRAJECTORY_MAX_ORDER 15

#define TRAJECTORY_FLAGS_T_REL (1 << 0)

typedef enum
{
  TRAJECTORY_TYPE_NO_MOVE,
  TRAJECTORY_TYPE_D,
  TRAJECTORY_TYPE_T,
} trajectoryType_t;

void trajectoryInit(void);
void trajectoryWait(void);

void _trajectoryNewOrder(trajectoryType_t type, double d, double a, double x, double y, double t, unsigned int flags);


#define TRAJECTORY_TEMP_S(tempo) _trajectoryNewOrder(TRAJECTORY_TYPE_T, 0, 0, 0, 0, tempo, TRAJECTORY_FLAGS_T_REL)

#endif //TRAJECTORY_H
