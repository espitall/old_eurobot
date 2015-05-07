#ifndef MECA_H
#define MECA_H

typedef enum 
{
  MECA_FOOT_LOW,
  MECA_FOOT_HIGH,
} meca_foot_t;

typedef enum
{
  MECA_CARPET_CLOSE,
  MECA_CARPET_OPEN_1,
  MECA_CARPET_OPEN_2,
  MECA_CARPET_OPEN_3,
} meca_carpet_t;

void mecaInit(void);

void mecaSetBackFoot(meca_foot_t pos);
void mecaSetFrontFoot(meca_foot_t pos);

void mecaSetRightCarpetState(meca_carpet_t state);
void mecaSetLeftCarpetState(meca_carpet_t state);

#endif// MECA_H

