#ifndef MECA_H
#define MECA_H

typedef enum 
{
  MECA_FOOT_LOW,
  MECA_FOOT_HIGH,
  MECA_FOOT_IDLE,
} meca_foot_t;

typedef enum 
{
  MECA_SUCKER_OFF,
  MECA_SUCKER_LOCKED,
  MECA_SUCKER_ON,
} meca_sucker_t;

typedef enum
{
  MECA_CARPET_CLOSE,
  MECA_CARPET_OPEN_1,
  MECA_CARPET_OPEN_2,
  MECA_CARPET_OPEN_2_BACK,
  MECA_CARPET_OPEN_3,

  MECA_CARPET2_CLOSE,
  MECA_CARPET2_OPEN_1,
  MECA_CARPET2_OPEN_2,
  MECA_CARPET2_OPEN_2_BACK,
  MECA_CARPET2_OPEN_3,

} meca_carpet_t;

void mecaInit(void);

void mecaSetBackFoot(meca_foot_t pos);
void mecaSetFrontFoot(meca_foot_t pos);

void mecaSetRightCarpetState(meca_carpet_t state);
void mecaSetLeftCarpetState(meca_carpet_t state);

void mecaSetSuckerState(int id, meca_sucker_t state);

#endif// MECA_H

