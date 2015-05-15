#include <pcm9685.h>
#include <lcd.h>
#include <ch.h>
#include "meca.h"

#define MECA_CARPET_RIGHT_ROTATION_ID           13
#define MECA_CARPET_RIGHT_FRONT_CLAMP_ID        15
#define MECA_CARPET_RIGHT_BACK_CLAMP_ID         14

#define MECA_CARPET_LEFT_ROTATION_ID            2
#define MECA_CARPET_LEFT_FRONT_CLAMP_ID         0
#define MECA_CARPET_LEFT_BACK_CLAMP_ID          1

#define MECA_CARPET_RIGHT_ROTATION_CLOSE        220
#define MECA_CARPET_RIGHT_ROTATION_OPEN         430
#define MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE     185
#define MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN      300
#define MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE      190
#define MECA_CARPET_RIGHT_BACK_CLAMP_OPEN       350

#define MECA_CARPET_LEFT_ROTATION_CLOSE         400
#define MECA_CARPET_LEFT_ROTATION_OPEN          200
#define MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE      410
#define MECA_CARPET_LEFT_FRONT_CLAMP_OPEN       200
#define MECA_CARPET_LEFT_BACK_CLAMP_CLOSE       350
#define MECA_CARPET_LEFT_BACK_CLAMP_OPEN        450

#define MECA_PUMP_ID                            8
#define MECA_SUCKER_0_ID                        3
#define MECA_SUCKER_1_ID                        4
#define MECA_SUCKER_2_ID                        5
#define MECA_SUCKER_AIR_ID                      6

#define MECA_FOOT_FRONT_LEFT_ID                 9
#define MECA_FOOT_BACK_LEFT_ID                  10
#define MECA_FOOT_FRONT_RIGHT_ID                11
#define MECA_FOOT_BACK_RIGHT_ID                 12

#define MECA_FOOT_BACK_LEFT_HIGH                495
#define MECA_FOOT_BACK_RIGHT_HIGH               480

#define MECA_FOOT_BACK_LEFT_LOW                430
#define MECA_FOOT_BACK_RIGHT_LOW               430
//#define MECA_FOOT_BACK_LEFT_LOW                300
//#define MECA_FOOT_BACK_RIGHT_LOW               300

#define MECA_FOOT_FRONT_LEFT_LOW                280
#define MECA_FOOT_FRONT_RIGHT_LOW               445

#define MECA_FOOT_FRONT_LEFT_HIGH                550
#define MECA_FOOT_FRONT_RIGHT_HIGH               200

//tatic WORKING_AREA(waMeca, 4096);
//static meca_sucker_t sucker_state[3];
//static MUTEX_DECL(mutex_sucker);

void mecaSetFrontFoot(meca_foot_t pos)
{
  switch(pos)
  {
    case MECA_FOOT_IDLE:
      pcm9685SetChannel(MECA_FOOT_FRONT_LEFT_ID, 0, 0);
      pcm9685SetChannel(MECA_FOOT_FRONT_RIGHT_ID, 0, 0);
      break;

    case MECA_FOOT_HIGH:
      pcm9685SetChannel(MECA_FOOT_FRONT_LEFT_ID, 0, 0 * MECA_FOOT_FRONT_LEFT_HIGH);
      pcm9685SetChannel(MECA_FOOT_FRONT_RIGHT_ID, 0, MECA_FOOT_FRONT_RIGHT_HIGH);
      pcm9685SetChannel(MECA_FOOT_FRONT_RIGHT_ID, 0, 0);
      break;

    case MECA_FOOT_LOW:
      pcm9685SetChannel(MECA_FOOT_FRONT_LEFT_ID, 0, MECA_FOOT_FRONT_LEFT_LOW);
      pcm9685SetChannel(MECA_FOOT_FRONT_RIGHT_ID, 0, MECA_FOOT_FRONT_RIGHT_LOW);
      break;

  }
}

void mecaSetBackFoot(meca_foot_t pos)
{
  switch(pos)
  {
    case MECA_FOOT_IDLE:
      pcm9685SetChannel(MECA_FOOT_BACK_LEFT_ID, 0, 0);
      pcm9685SetChannel(MECA_FOOT_BACK_RIGHT_ID, 0, 0);
      break;
      
    case MECA_FOOT_HIGH:
      pcm9685SetChannel(MECA_FOOT_BACK_LEFT_ID, 0, MECA_FOOT_BACK_LEFT_HIGH * 0 + 310);
      pcm9685SetChannel(MECA_FOOT_BACK_RIGHT_ID, 0, MECA_FOOT_BACK_RIGHT_HIGH);
      break;

    case MECA_FOOT_LOW:
      pcm9685SetChannel(MECA_FOOT_BACK_LEFT_ID, 0, MECA_FOOT_BACK_LEFT_LOW);
      pcm9685SetChannel(MECA_FOOT_BACK_RIGHT_ID, 0, MECA_FOOT_BACK_RIGHT_LOW);
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
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET_OPEN_2_BACK:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_3:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_CLOSE:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_OPEN_1:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_OPEN_2:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET2_OPEN_2_BACK:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_OPEN_3:
      pcm9685SetChannel(MECA_CARPET_RIGHT_ROTATION_ID,    0, MECA_CARPET_RIGHT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_FRONT_CLAMP_ID, 0, MECA_CARPET_RIGHT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_RIGHT_BACK_CLAMP_ID,  0, MECA_CARPET_RIGHT_BACK_CLAMP_CLOSE);
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
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET_OPEN_2_BACK:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET_OPEN_3:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_OPEN);
      break;

  case MECA_CARPET2_CLOSE:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_OPEN_1:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_OPEN_2:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_CLOSE);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_CLOSE);
      break;

    case MECA_CARPET2_OPEN_2_BACK:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_OPEN);
      break;

    case MECA_CARPET2_OPEN_3:
      pcm9685SetChannel(MECA_CARPET_LEFT_ROTATION_ID,    0, MECA_CARPET_LEFT_ROTATION_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_FRONT_CLAMP_ID, 0, MECA_CARPET_LEFT_FRONT_CLAMP_OPEN);
      pcm9685SetChannel(MECA_CARPET_LEFT_BACK_CLAMP_ID,  0, MECA_CARPET_LEFT_BACK_CLAMP_CLOSE);
      break;
  }
}

//static msg_t mecaThread(void *arg) 
//{
//  (void)arg;
//
//  int offset = 0;
//  while(1)
//  {
//    chMtxLock(&mutex_sucker);
//    if((sucker_state[0] == MECA_SUCKER_ON) || (sucker_state[1] == MECA_SUCKER_ON) || (sucker_state[2] == MECA_SUCKER_ON)) 
//    {
//      pcm9685SetChannel(MECA_PUMP_ID, 0, 4095);
//      chThdSleepMilliseconds(500);
//      int i;
//      for(i = 0; i < 3; i += 1)
//      {
//        int j = i + offset;
//        j %= 3;
//        if(sucker_state[j] == MECA_SUCKER_ON)
//        {
//          lcdPrintln(LCD_INFO, "s%d",j);
//          pcm9685SetChannel(MECA_SUCKER_0_ID + j, 0, 4095);
//          chThdSleepMilliseconds(500);
//          pcm9685SetChannel(MECA_SUCKER_0_ID + j, 0, 0);
//          chThdSleepMilliseconds(500);
//        }
//      }
//      offset += 1;
//
//      /*
//      if(sucker_state[0] == MECA_SUCKER_ON)
//      {
//        lcdPrintln(LCD_INFO, "s0");
//        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 4095);
//        chThdSleepMilliseconds(500);
//        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 0);
//        chThdSleepMilliseconds(500);
//      }
//      if(sucker_state[1] == MECA_SUCKER_ON)
//      {
//        lcdPrintln(LCD_INFO, "s1");
//        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 4095);
//        chThdSleepMilliseconds(500);
//        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 0);
//        chThdSleepMilliseconds(500);
//      }
//      if(sucker_state[2] == MECA_SUCKER_ON)
//      {
//        lcdPrintln(LCD_INFO, "s2");
//        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 4095);
//        chThdSleepMilliseconds(500);
//        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 0);
//        chThdSleepMilliseconds(500);
//      }
//      */
//      pcm9685SetChannel(MECA_PUMP_ID, 0, 0);
//      chThdSleepMilliseconds(100);
//    }
//    chMtxUnlock();
//
//
//
//    chMtxLock(&mutex_sucker);
//    if((sucker_state[0] == MECA_SUCKER_OFF) || (sucker_state[1] == MECA_SUCKER_OFF) || (sucker_state[2] == MECA_SUCKER_OFF)) 
//    {
//      pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 4095);
//      if(sucker_state[0] == MECA_SUCKER_OFF)
//      {
//        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 4095);
//      }
//      if(sucker_state[1] == MECA_SUCKER_OFF)
//      {
//        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 4095);
//      }
//      if(sucker_state[2] == MECA_SUCKER_OFF)
//      {
//        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 4095);
//      }
//      chThdSleepMilliseconds(200);
//
//      pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 0);
//      pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 0);
//      pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 0);
//      pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 0);
//    }
//    chMtxUnlock();
//
//  }
//
//  return 0;
//}

void mecaSetSuckerState(int id, meca_sucker_t state)
{
  int pwmid = 0;

  switch(id)
  {
    case 0:
      pwmid = MECA_SUCKER_0_ID;
      break;

    case 1:
      pwmid = MECA_SUCKER_1_ID;
      break;

    case 2:
      pwmid = MECA_SUCKER_2_ID;
      break;
  }

  switch(state)
  {
    case MECA_SUCKER_ON:
      pcm9685SetChannel(pwmid, 0, 4095);
      chThdSleepMilliseconds(500);
      pcm9685SetChannel(MECA_PUMP_ID, 0, 4095);
      break;

    case MECA_SUCKER_LOCKED:
      pcm9685SetChannel(pwmid, 0, 0);
      chThdSleepMilliseconds(500);
      pcm9685SetChannel(MECA_PUMP_ID, 0, 0);
      break;

    case MECA_SUCKER_OFF:
      pcm9685SetChannel(pwmid, 0, 4095);
      chThdSleepMilliseconds(500);
      pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 4095);
      chThdSleepMilliseconds(1500);
      pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 0);
      pcm9685SetChannel(pwmid, 0, 0);
      break;


  }

  /*
  sucker_state[id] = state;
  return;

  chMtxLock(&mutex_sucker);
  
  if(state == MECA_SUCKER_ON)
  {
    pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 0);
    chThdSleepMilliseconds(100);
    switch(id)
    {
      case 0:
        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 4095);
        pcm9685SetChannel(MECA_PUMP_ID, 0, 4095);
        chThdSleepMilliseconds(2000);
        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 0);
        chThdSleepMilliseconds(50);
        break;

      case 1:
        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 4095);
    pcm9685SetChannel(MECA_PUMP_ID, 0, 4095);
        chThdSleepMilliseconds(2000);
        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 0);
        chThdSleepMilliseconds(50);
        break;

      case 2:
        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 4095);
    pcm9685SetChannel(MECA_PUMP_ID, 0, 4095);
        chThdSleepMilliseconds(2000);
        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 0);
        chThdSleepMilliseconds(50);
        break;
    }
    pcm9685SetChannel(MECA_PUMP_ID, 0, 0);
  }

  if(state == MECA_SUCKER_OFF)
  {
    pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 4095);
    pcm9685SetChannel(MECA_PUMP_ID, 0, 0);
    chThdSleepMilliseconds(100);
    switch(id)
    {
      case 0:
        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 4095);
        chThdSleepMilliseconds(1000);
        pcm9685SetChannel(MECA_SUCKER_0_ID, 0, 0);
        chThdSleepMilliseconds(50);
        break;

      case 1:
        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 4095);
        chThdSleepMilliseconds(1000);
        pcm9685SetChannel(MECA_SUCKER_1_ID, 0, 0);
        chThdSleepMilliseconds(50);
        break;

      case 2:
        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 4095);
        chThdSleepMilliseconds(1000);
        pcm9685SetChannel(MECA_SUCKER_2_ID, 0, 0);
        chThdSleepMilliseconds(50);
        break;
    }
    pcm9685SetChannel(MECA_SUCKER_AIR_ID, 0, 0);
  }

  chMtxUnlock();
*/
}

void mecaInit(void)
{
 // chMtxInit(&mutex_sucker);

  int i;
  for(i = 0; i < 16; i += 1)
  {
    pcm9685SetChannel(i, 0, 0);
  }

//  for(i = 0; i < 3; i += 1)
//  {
//    sucker_state[i] = MECA_SUCKER_OFF;
//  }

  mecaSetBackFoot(MECA_FOOT_HIGH);
  mecaSetFrontFoot(MECA_FOOT_LOW);

  mecaSetRightCarpetState(MECA_CARPET_OPEN_3);
  mecaSetLeftCarpetState(MECA_CARPET_OPEN_3);

 // chThdCreateStatic(waMeca, sizeof(waMeca), NORMALPRIO, mecaThread, NULL);
}
