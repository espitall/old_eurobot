#include <ch.h>
#include <hal.h>
#include <usir.h>
#include <hal.h>
#include <string.h>
#include <math.h>
#include <lcd.h>
#include <strat.h>
#include <max7317.h>
#include "trajectory.h"
#include "asserv.h"
#include "position.h"
#include "config.h"

typedef enum
{
  TRAJECTORY_RESULT_NOTHING,
  TRAJECTORY_RESULT_REMOVE,
} trajectoryResult_t;


typedef struct
{
  trajectoryType_t type;

  double DSetPointmm;
  double ASetPointdeg;
  double XSetPointmm;
  double YSetPointmm;
  systime_t TSetPointt;

  unsigned int flags;

  double DStartmm;
  double AStartdeg;
  double XStartmm;
  double YStartmm;
  systime_t TStartt;

  double DSafetymm;
} trajectory_t;


static MUTEX_DECL(mutex);
static CONDVAR_DECL(condVar);
static WORKING_AREA(waTrajThread, 2048);
static trajectory_t orderList[TRAJECTORY_MAX_ORDER];
static int writePosition;
static int readPosition;
static double safety;
static trajectoryStatus_t status;

static trajectoryResult_t trajectory_handle_type_no_move(trajectory_t * traj)
{

  asservSetDistanceSetPoint(traj->DStartmm);
  asservSetAngularSetPoint(traj->AStartdeg);

  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_d(trajectory_t * traj)
{
  if(traj->flags & TRAJECTORY_FLAGS_JUST_INIT)
  {
    traj->flags  &= ~TRAJECTORY_FLAGS_JUST_INIT;
    if(safety > 0)
    {
      if(traj->DSetPointmm >= 0)
      {
        usirSetSafetymm(0x03, safety);
      }
      else
      {
        usirSetSafetymm(0x0c, safety);
      }
    }
  }

 double l = fabs(traj->DStartmm + traj->DSetPointmm - posGetDmm());
  if(l < 3.0)
  {
    return TRAJECTORY_RESULT_REMOVE;
  }

  if(safety > 0)
  {
    if(palReadPad(GPIOD, GPIOD_PIN5))
    {
      if(!(traj->flags & TRAJECTORY_FLAGS_SAFETY))
      {
        traj->flags = TRAJECTORY_FLAGS_SAFETY;
        traj->DSafetymm = posGetDmm();
      }

      asservSetDistanceSetPointSafety(traj->DSafetymm);
      return TRAJECTORY_RESULT_NOTHING;
    }
    else
    {
        traj->flags  &= ~TRAJECTORY_FLAGS_SAFETY;
    }
  }


  asservSetDistanceSetPoint(traj->DStartmm + traj->DSetPointmm);

  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_wedging(trajectory_t * traj)
{
  (void)traj;

  asservSetDistanceSetPoint(posGetDmm() - 10);
  uint16_t switchs = ~max7317Read();

#ifdef PMI
  switchs = ~switchs;
#endif

  switch(switchs & ((1 << IO_SWITCH_BACK_LEFT) | (1 << IO_SWITCH_BACK_RIGHT)))
  {
    case ((1 << IO_SWITCH_BACK_LEFT) | (1 << IO_SWITCH_BACK_RIGHT)):
      asservSetDistanceSetPoint(posGetDmm() - 5);
      asservSetAngularSetPoint(posGetAdeg());
      return TRAJECTORY_RESULT_REMOVE;

    case (1 << IO_SWITCH_BACK_LEFT):
      asservSetAngularSetPoint(posGetAdeg() - 3);
      break;

    case (1 << IO_SWITCH_BACK_RIGHT):
      asservSetAngularSetPoint(posGetAdeg() + 3);
      break;

    default:
      asservSetAngularSetPoint(posGetAdeg());
      break;
  }

  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_xy(trajectory_t * traj)
{
  (void) traj;

  double dx = traj->XSetPointmm - posGetXmm();
  double dy = traj->YSetPointmm - posGetYmm();
  double dd = dx*dx + dy*dy;

  if(traj->flags & TRAJECTORY_FLAGS_JUST_INIT)
  {
    traj->flags  &= ~TRAJECTORY_FLAGS_JUST_INIT;
    if(safety > 0)
    {
      usirSetSafetymm(0x03, safety);
    }
  }

  if(dd < 25) 
  {
    return TRAJECTORY_RESULT_REMOVE;
  }
  else
  {
    if(safety > 0)
    {
      if(palReadPad(GPIOD, GPIOD_PIN5))
      {
        if(!(traj->flags & TRAJECTORY_FLAGS_SAFETY))
        {
          traj->flags = TRAJECTORY_FLAGS_SAFETY;
          traj->DSafetymm = posGetDmm();
        }

        asservSetDistanceSetPointSafety(traj->DSafetymm);
        return TRAJECTORY_RESULT_NOTHING;
      }
      else
      {
        traj->flags  &= ~TRAJECTORY_FLAGS_SAFETY;
      }
    }

    double cur = posGetAdeg();
    double target_deg = atan2(dy, dx) * 180.0 / M_PI;

    double abase = floor(cur / 360.0) * 360.0;

    while(target_deg > 180.0) {
      target_deg -= 360.0;
    }
    while(target_deg < -180.0) {
      target_deg += 360.0;
    }

    double t1 = abase + target_deg;
    double t2 = abase + target_deg + 360.0;
    double t3 = abase + target_deg - 360.0;

    double dt1 = fabs(t1 - cur);
    double dt2 = fabs(t2 - cur);
    double dt3 = fabs(t3 - cur);

    double target = t1;
    if(dt2 < dt1)
    {
      target = t2;
      if(dt3 < dt2)
      {
        target = t3;
      }
    }
    else if(dt3 < dt1)
    {
      target = t3;
    }
    if(fabs(target - cur) < 60) 
    {
      asservSetDistanceSetPoint(posGetDmm() + sqrt(dd));
    }
    else
    {
      asservSetDistanceSetPoint(posGetDmm());
    }

    asservSetAngularSetPoint(target);
  }


  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_xy_back(trajectory_t * traj)
{
  (void) traj;

  double dx = traj->XSetPointmm - posGetXmm();
  double dy = traj->YSetPointmm - posGetYmm();
  double dd = dx*dx + dy*dy;

  if(traj->flags & TRAJECTORY_FLAGS_JUST_INIT)
  {
    traj->flags  &= ~TRAJECTORY_FLAGS_JUST_INIT;
    if(safety > 0)
    {
      usirSetSafetymm(0x0C, safety);
    }
  }

  if(dd < 25) 
  {
    return TRAJECTORY_RESULT_REMOVE;
  }
  else
  {
    if(safety > 0)
    {
      if(palReadPad(GPIOD, GPIOD_PIN5))
      {
        if(!(traj->flags & TRAJECTORY_FLAGS_SAFETY))
        {
          traj->flags = TRAJECTORY_FLAGS_SAFETY;
          traj->DSafetymm = posGetDmm();
        }

        asservSetDistanceSetPointSafety(traj->DSafetymm);
        return TRAJECTORY_RESULT_NOTHING;
      }
      else
      {
        traj->flags  &= ~TRAJECTORY_FLAGS_SAFETY;
      }
    }

    double cur = posGetAdeg();
    double target_deg = atan2(dy, dx) * 180.0 / M_PI + 180.0;

    double abase = floor(cur / 360.0) * 360.0;

    while(target_deg > 180.0) {
      target_deg -= 360.0;
    }
    while(target_deg < -180.0) {
      target_deg += 360.0;
    }

    double t1 = abase + target_deg;
    double t2 = abase + target_deg + 360.0;
    double t3 = abase + target_deg - 360.0;

    double dt1 = fabs(t1 - cur);
    double dt2 = fabs(t2 - cur);
    double dt3 = fabs(t3 - cur);

    double target = t1;
    if(dt2 < dt1)
    {
      target = t2;
      if(dt3 < dt2)
      {
        target = t3;
      }
    }
    else if(dt3 < dt1)
    {
      target = t3;
    }
    if(fabs(target - cur) < 60) 
    {
      asservSetDistanceSetPoint(posGetDmm() - sqrt(dd));
    }
    else
    {
      asservSetDistanceSetPoint(posGetDmm());
    }

    asservSetAngularSetPoint(target);
  }


  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_a(trajectory_t * traj)
{
  double abase = floor(traj->AStartdeg / 360.0) * 360.0;

  while(traj->ASetPointdeg > 180.0) {
    traj->ASetPointdeg -= 360.0;
  }
  while(traj->ASetPointdeg < -180.0) {
    traj->ASetPointdeg += 360.0;
  }

  double t1 = abase + traj->ASetPointdeg;
  double t2 = abase + traj->ASetPointdeg + 360.0;
  double t3 = abase + traj->ASetPointdeg - 360.0;

  double dt1 = fabs(t1 - traj->AStartdeg);
  double dt2 = fabs(t2 - traj->AStartdeg);
  double dt3 = fabs(t3 - traj->AStartdeg);

  double target = t1;
  if(dt2 < dt1)
  {
    target = t2;
    if(dt3 < dt2)
    {
      target = t3;
    }
  }
  else if(dt3 < dt1)
  {
    target = t3;
  }

  if(fabs(target - posGetAdeg()) < 3.0)
  {
    return TRAJECTORY_RESULT_REMOVE;
  }

  asservSetAngularSetPoint(target);

  return TRAJECTORY_RESULT_NOTHING;
}

static trajectoryResult_t trajectory_handle_type_t(trajectory_t * traj)
{

  if((chTimeNow() - traj->TStartt) > traj->TSetPointt)
  {
    return TRAJECTORY_RESULT_REMOVE;
  }


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
      trajectoryResult_t result = TRAJECTORY_RESULT_NOTHING;

      if(isnan(traj->DStartmm))
      {
        traj->DStartmm = posGetDmm();
        traj->AStartdeg = posGetAdeg();
        traj->XStartmm = posGetXmm();
        traj->YStartmm = posGetYmm();
        traj->TStartt = chTimeNow();
        traj->flags = TRAJECTORY_FLAGS_JUST_INIT;
      }

      switch(traj->type)
      {
        case TRAJECTORY_TYPE_NO_MOVE:
          result = trajectory_handle_type_no_move(traj);
          break;

        case TRAJECTORY_TYPE_D:
          result = trajectory_handle_type_d(traj);
          break;

        case TRAJECTORY_TYPE_A:
          result = trajectory_handle_type_a(traj);
          break;

        case TRAJECTORY_TYPE_T:
          result = trajectory_handle_type_t(traj);
          break;

        case TRAJECTORY_TYPE_XY_BACK:
          result = trajectory_handle_type_xy_back(traj);
          break;

        case TRAJECTORY_TYPE_XY:
          result = trajectory_handle_type_xy(traj);
          break;

        case TRAJECTORY_TYPE_WEDGING:
          result = trajectory_handle_type_wedging(traj);
          break;
      }

      if((chTimeNow() - traj->TStartt) > traj->TSetPointt)
      {
        result = TRAJECTORY_RESULT_REMOVE;
        if(traj->type != TRAJECTORY_TYPE_T)
        {
          status = TRAJECTORY_FAILED_TEMPO;
        }
      }

      if(result == TRAJECTORY_RESULT_REMOVE)
      {
        readPosition = (readPosition + 1) % TRAJECTORY_MAX_ORDER;
      }
    }
    else
    {
      chCondBroadcast(&condVar);
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
  t->ASetPointdeg = 0;
  t->XSetPointmm = 0;
  t->YSetPointmm = 0;
  t->TSetPointt = 0;

  t->DStartmm = NAN;
  t->AStartdeg = NAN;
  t->XStartmm = NAN;
  t->YStartmm = NAN;
  t->TStartt = 0;

  t->flags = 0;
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
  safety = 0;
  status = TRAJECTORY_OK;

  chMtxInit(&mutex);
  chCondInit(&condVar);

  chThdCreateStatic(waTrajThread, sizeof(waTrajThread), POSITION_TRAJECTORY_PRIO, trajectoryThread, NULL);
}

trajectoryStatus_t trajectoryWait(void)
{
  chMtxLock(&mutex);
  while(readPosition != writePosition)
  {
    chCondWait(&condVar);
  }
  chMtxUnlock();

  return status;
}

void _trajectoryNewOrder(trajectoryType_t type, double d, double a, double x, double y, double t, unsigned int flags)
{
  trajectory_t traj;
  trajectoryInitializeStruct(&traj);

  if(t <= 0.1)
  {
    t = 90;
  }

  traj.type = type;
  traj.DSetPointmm = d;
  traj.ASetPointdeg = a;
  traj.XSetPointmm = x;
  traj.YSetPointmm = y;
  traj.TSetPointt = S2ST(t);
  traj.flags = flags;

  if(stratGetColor() == STRAT_COLOR_GREEN)
  {
    traj.ASetPointdeg = 180 - traj.ASetPointdeg;
    traj.XSetPointmm = - traj.XSetPointmm;
  }

  status = TRAJECTORY_OK;
  trajectoryAddToOrderList(&traj);
}

void trajectoryPrint(void)
{
  int i = readPosition;
  while(i != writePosition)
  {
    trajectory_t * traj = &orderList[i];
    switch(traj->type)
    {
      case TRAJECTORY_TYPE_XY:
      case TRAJECTORY_TYPE_XY_BACK:
        {
          lcd2DPoint_t tmp;
          tmp.x = traj->XSetPointmm;
          tmp.y = traj->YSetPointmm;
          lcdCircle(tmp, 50, 0, 0, LCD_COLOR(255, 0, 0), LCD_METRIC | LCD_FIELD);
        }
        break;

      default:
        break;
    }

    i += 1;
    i %= TRAJECTORY_MAX_ORDER;
  }
}

void trajectorySetSafetymm(double d)
{
  safety = d;
}
