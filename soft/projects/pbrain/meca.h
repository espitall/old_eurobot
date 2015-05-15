#ifndef MECA_H
#define MECA_H

typedef enum
{
  MECA_ARM_UP,
  MECA_ARM_DOWN,
} mecaArm_t;

void mecaInit(void);
void mecaSetLeftArm(mecaArm_t status);
void mecaSetRightArm(mecaArm_t status);
void mecaDisable(void);

#endif// MECA_H
