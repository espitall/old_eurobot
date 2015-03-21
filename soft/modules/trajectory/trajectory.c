#include <ch.h>
#include <hal.h>
#include <string.h>
#include <math.h>
#include "trajectory.h"
#include "asserv.h"
#include "position.h"
#include "config.h"

typedef enum
{
  TRAJECTORY_TYPE_NO_MOVE,
  TRAJECTORY_TYPE_D,
} trajectoryType_t;

typedef enum
{
  TRAJECTORY_RESULT_NOTHING,
  TRAJECTORY_RESULT_REMOVE,
} trajectoryResult_t;


typedef struct
{
  trajectoryType_t type;

  int started;
  
  double DSetPointmm;

  double DStartmm;
  double AStartdeg;

} trajectory_t;


static Semaphore sem;
static Mutex mutex;
static WORKING_AREA(waTrajThread, 256);
static trajectory_t orderList[TRAJECTORY_MAX_ORDER];
static int writePosition;
static int readPosition;

static trajectoryResult_t trajectory_handle_type_no_move(trajectory_t * traj)
{

  asservSetDistanceSetPoint(traj->DStartmm);
  asservSetAngularSetPoint(traj->AStartdeg);

  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_d(trajectory_t * traj)
{

  if(fabs(traj->DStartmm + traj->DSetPointmm - posGetDmm()) < 1.0)
  {
    return TRAJECTORY_RESULT_REMOVE;
  }

  asservSetDistanceSetPoint(traj->DStartmm + traj->DSetPointmm);

  return TRAJECTORY_RESULT_NOTHING;
}

static msg_t trajectoryThread(void *arg) 
{
  (void) arg;

  systime_t time = chTimeNow();

  while(TRUE)
  {
    time += MS2ST(100);
    chMtxLock(&mutex);
    if(readPosition != writePosition)
    {
      trajectory_t * traj = &orderList[readPosition];
      trajectoryResult_t result = TRAJECTORY_RESULT_NOTHING;;

      if(isnan(traj->DStartmm))
      {
        traj->DStartmm = posGetDmm();
      }
      if(isnan(traj->AStartdeg))
      {
        traj->AStartdeg = posGetAdeg();
      }

      switch(traj->type)
      {
        case TRAJECTORY_TYPE_NO_MOVE:
          result = trajectory_handle_type_no_move(traj);
          break;

        case TRAJECTORY_TYPE_D:
          result = trajectory_handle_type_d(traj);
          break;
      }

      if(result == TRAJECTORY_RESULT_REMOVE)
      {
        readPosition = (readPosition + 1) % TRAJECTORY_MAX_ORDER;
      }
    }
    chMtxUnlock();
    chThdSleepUntil(time);
  }

  return 0;
}

static void trajectoryInitializeStruct(trajectory_t * t)
{
  t->type = TRAJECTORY_TYPE_NO_MOVE;

  t->DSetPointmm = 0;

  t->DStartmm = NAN;
  t->AStartdeg = NAN;
}

static void trajectoryAddToOrderList(trajectory_t * t)
{
  chMtxLock(&mutex);
  int nextPosition = (writePosition + 1) % TRAJECTORY_MAX_ORDER;
  if(nextPosition != readPosition)
  {
    memcpy(&orderList[writePosition], t, sizeof(trajectory_t));
  }

  writePosition = nextPosition;
  chMtxUnlock();
}

void trajectoryInit(void)
{
  writePosition = 0;
  readPosition = 0;

  chSemInit(&sem, 1);

  chThdCreateStatic(waTrajThread, sizeof(waTrajThread), POSITION_TRAJECTORY_PRIO, trajectoryThread, NULL);
}

void trajectoryWait(void)
{
  chSemWait(&sem);
  chSemSignal(&sem);
}

void trajectoryGotoDmm(double d)
{
  trajectory_t traj;
  trajectoryInitializeStruct(&traj);

  traj.type = TRAJECTORY_TYPE_D;
  traj.DSetPointmm = d;

  trajectoryAddToOrderList(&traj);
}
