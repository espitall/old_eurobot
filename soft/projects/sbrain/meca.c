#include <pcm9685.h>
#include <ch.h>
#include "meca.h"

#define MECA_CARPET_RIGHT_ROTATION_ID           13
#define MECA_CARPET_RIGHT_FRONT_CLAMP_ID        15
#define MECA_CARPET_RIGHT_BACK_CLAMP_ID         14

#define MECA_CARPET_LEFT_ROTATION_ID            0
#define MECA_CARPET_LEFT_FRONT_CLAMP_ID         0
#define MECA_CARPET_LEFT_BACK_CLAMP_ID          0

#define MECA_CARPET_RIGHT_ROTATION_CLOSE        220
#define MECA_CARPET_RIGHT_ROTATION_OPEN         430
#define MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE     185
#define MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN      300
#define MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE      535
#define MECA_CARPET_RIGHT_BACK_CLAMP_OPEN       350

#define MECA_CARPET_LEFT_ROTATION_CLOSE         350
#define MECA_CARPET_LEFT_ROTATION_OPEN          350
#define MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE      350
#define MECA_CARPET_LEFT_FRONT_CLAMP_OPEN       350
#define MECA_CARPET_LEFT_BACK_CLAMP_CLOSE       350
#define MECA_CARPET_LEFT_BACK_CLAMP_OPEN        350

void mecaSetFrontFoot(meca_foot_t pos)
{
  switch(pos)
  {
    case MECA_FOOT_HIGH:
      pcm9685SetChannel(8, 0, 500);
      pcm9685SetChannel(10, 0, 200);
      break;

    case MECA_FOOT_LOW:
      pcm9685SetChannel(8, 0, 300);
      pcm9685SetChannel(10, 0, 400);
      break;

  }
}

void mecaSetBackFoot(meca_foot_t pos)
{
  switch(pos)
  {
    case MECA_FOOT_HIGH:
      pcm9685SetChannel(9, 0, 450);
      pcm9685SetChannel(11, 0, 450);
      break;

    case MECA_FOOT_LOW:
      pcm9685SetChannel(9, 0, 425);
      pcm9685SetChannel(11, 0, 425);
      break;

  }
}

void mecaSetRightCarpetState(meca_carpet_t state)
{
  switch(state)
  {
    case MECA_CARPET_CLOSE:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_1:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_2:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_3:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_OPEN);
      break;
  }
}

void mecaSetLeftCarpetState(meca_carpet_t state)
{
  switch(state)
  {
    case MECA_CARPET_CLOSE:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_1:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_2:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_3:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_OPEN);
      break;
  }
}

void mecaInit(void)
{
  pcm9685SetChannel(8, 0, 220);
  pcm9685SetChannel(9, 0, 500);
  pcm9685SetChannel(10, 0, 450);
  pcm9685SetChannel(11, 0, 550);

  mecaSetBackFoot(MECA_FOOT_HIGH);
  mecaSetFrontFoot(MECA_FOOT_LOW);
 // mecaSetFrontFoot(MECA_FOOT_HIGH);

  //pcm9685SetChannel(8, 0, 0);
  //pcm9685SetChannel(9, 0, 0);
  //pcm9685SetChannel(10, 0, 0);
  //pcm9685SetChannel(11, 0, 0);

  mecaSetLeftCarpetState(MECA_CARPET_OPEN_3);
  mecaSetRightCarpetState(MECA_CARPET_OPEN_3);
  
}
