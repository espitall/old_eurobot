#include <ch.h>
#include <hal.h>
#include <math.h>
#include <lcd.h>
#include "position.h"
#include "config.h"
#include "asserv.h"

#define TICK2RAD(tick)  ((((double)tick) * M_PI) / TICK_PER_180DEG) 
#define TICK2DEG(tick)  ((((double)tick) * 180.0) / TICK_PER_180DEG) 
#define TICK2MM(tick)  ((((double)tick) * 1000.0) / TICK_PER_1M) 
#define MM2TICK(mm)  ((((double)mm) * TICK_PER_1M) / 1000.0) 

static WORKING_AREA(waPosThread, 2048);

static Mutex _mutex;

static int32_t _enc_value[2];
static uint16_t _enc_last_angle[2];
static int32_t _enc_distance;
static int32_t _enc_angle;
static int32_t _enc_angle_offset;
static double _enc_x;
static double _enc_y;

static void (*_position_computed_hook)(void);

void posComputeEncoderPosition(int enc_id)
{
  //lock SPI bus
  spiAcquireBus(&SPID4);

  uint16_t raw;

  //lecture du bus SPI
  if(enc_id == 0)
  {
    palClearPad(GPIOE, GPIOE_SPI4_ENC0_CS);
  }
  else
  {
    palClearPad(GPIOC, GPIOC_SPI4_ENC1_CS);
  }
  spiReceive(&SPID4, 2, &raw);

  if(enc_id == 0)
  {
    palSetPad(GPIOE, GPIOE_SPI4_ENC0_CS);
  }
  else
  {
    palSetPad(GPIOC, GPIOC_SPI4_ENC1_CS);
  }

  //libere les ressources
  spiReleaseBus(&SPID4);

  //swap octets + suppression MSB
  raw = ((raw & 0xff) << 8) | (raw >> 8);
  raw <<= 1;

  //calcul de la nouvelle position
  int16_t delta = raw - _enc_last_angle[enc_id];
  _enc_last_angle[enc_id] = raw;
  if(enc_id == 0)
  {
    _enc_value[enc_id] += delta;
  }
  else
  {
    _enc_value[enc_id] -= delta;
  }
}

static msg_t posThread(void *arg) 
{
  (void) arg;

  //recupere "l'heure" actuelle
  systime_t time = chTimeNow();

  while (TRUE) {
    //calcul l'�cheance du nouveau reveil
    time += MS2ST(10);

    chMtxLock(&_mutex);

    //mise a jour donnees codeurs
    posComputeEncoderPosition(0);
    posComputeEncoderPosition(1);
    
    //calcul de la distance
    int32_t new_distance = _enc_value[0] + _enc_value[1];
    int32_t delta_disance = new_distance - _enc_distance;

    //calcul de l'angle
    int32_t new_angle = _enc_value[0] - _enc_value[1] + _enc_angle_offset;      
    int32_t delta_angle = new_angle - _enc_angle;

    //calcul position X et Y
    double mid_angle = TICK2RAD(_enc_angle + delta_angle / 2);
    double dx = delta_disance * cos(mid_angle);
    double dy = delta_disance * sin(mid_angle);
    _enc_x += dx;
    _enc_y += dy;

    _enc_angle = new_angle;
    _enc_distance = new_distance;

    chMtxUnlock();
    asservCompute ();

    //attend la prochaine echeance
    chThdSleepUntil(time);
  }
  return 0;
}

void posInit(void (*position_computed_hook)(void))
{
  _position_computed_hook = position_computed_hook;

  //initialisation du bus SPI
  palSetPad(GPIOE, GPIOE_SPI4_ENC0_CS);
  palSetPad(GPIOC, GPIOC_SPI4_ENC1_CS);

  //initialisation des variables internes
  chMtxInit(&_mutex);
  chMtxLock(&_mutex);
  posComputeEncoderPosition(0);
  posComputeEncoderPosition(1);
  posComputeEncoderPosition(0);
  posComputeEncoderPosition(1);
  _enc_value[0] = 0;
  _enc_value[1] = 0;
  _enc_distance = 0;
  _enc_angle = 0;
  _enc_x = MM2TICK(0);
  _enc_y = MM2TICK(0);
  chMtxUnlock();

  //lancement du calcul periodique de la position
  chThdCreateStatic(waPosThread, sizeof(waPosThread), POSITION_SCHEDULER_PRIO, posThread, NULL);
}

double posGetAdeg(void)
{
  chMtxLock(&_mutex);
  double a = TICK2DEG(_enc_angle);
  chMtxUnlock();

  return a;
}

double posGetXmm(void)
{
  chMtxLock(&_mutex);
  double x = TICK2MM(_enc_x);
  chMtxUnlock();

  return x;
}

double posGetYmm(void)
{
  chMtxLock(&_mutex);
  double y = TICK2DEG(_enc_y);
  chMtxUnlock();

  return y;
}

double posGetDmm(void)
{
  chMtxLock(&_mutex);
  double d = TICK2MM(_enc_distance);
  chMtxUnlock();

  return d;
}

void posPrint(void)
{
  lcd2DPoint_t a,b;

  double x = posGetXmm();
  double y = posGetXmm();

  a.x = x - ROBOT_X / 2;
  a.y = y - ROBOT_Y / 2;
  b.x = x + ROBOT_X / 2;
  b.y = y + ROBOT_Y / 2;
  lcdRect(a, b, LCD_COLOR(0, 0, 255), LCD_METRIC | LCD_FIELD);
}
