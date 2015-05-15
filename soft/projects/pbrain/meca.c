#include <pcm9685.h>
#include "meca.h"

void mecaInit(void)
{
  mecaSetLeftArm(MECA_ARM_DOWN);
  mecaSetRightArm(MECA_ARM_DOWN);
}

void mecaSetRightArm(mecaArm_t status)
{
  switch(status)
  {
    case MECA_ARM_DOWN:
      pcm9685SetChannel(7, 0, 410);
      break;

    case MECA_ARM_UP:
      pcm9685SetChannel(7, 0, 200);
      break;
  }
}
void mecaSetLeftArm(mecaArm_t status)
{
  switch(status)
  {
    case MECA_ARM_DOWN:
      pcm9685SetChannel(6, 0, 200);
      break;

    case MECA_ARM_UP:
      pcm9685SetChannel(6, 0, 410);
      break;
  }
}

void mecaDisable(void)
{
  pcm9685SetChannel(6, 0, 0);
  pcm9685SetChannel(7, 0, 0);
}
