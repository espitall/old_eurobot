#include <ch.h>
#include <hal.h>
#include <math.h>
#include "position.h"
#include "config.h"

#define TICK2RAD(tick)  ((((double)tick) * M_PI) / TICK_PER_180DEG) 
#define TICK2DEG(tick)  ((((double)tick) * 180.0) / TICK_PER_180DEG) 
#define TICK2MM(tick)  ((((double)tick) * 1000.0) / TICK_PER_1M) 

static WORKING_AREA(waPosThread, 256);

static Mutex mutex;

static int32_t enc_value[2];
static uint16_t enc_last_angle[2];
static int32_t enc_distance;
static int32_t enc_angle;
static int32_t enc_angle_offset;
static double enc_x;
static double enc_y;

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
  int16_t delta = raw - enc_last_angle[enc_id];
  enc_last_angle[enc_id] = raw;
  enc_value[enc_id] += delta;
}

static msg_t posThread(void *arg) 
{
  (void) arg;

  //recupere "l'heure" actuelle
  systime_t time = chTimeNow();

  while (TRUE) {
    //calcul l'écheance du nouveau reveil
    time += MS2ST(10);

    chMtxLock(&mutex);

    //mise a jour donnees codeurs
    posComputeEncoderPosition(0);
    posComputeEncoderPosition(1);
    
    //calcul de la distance
    int32_t new_distance = enc_value[0] + enc_value[1];
    int32_t delta_disance = new_distance - enc_distance;

    //calcul de l'angle
    int32_t new_angle = enc_value[0] - enc_value[1] + enc_angle_offset;      
    int32_t delta_angle = new_angle - enc_angle;

    //calcul position X et Y
    double mid_angle = TICK2RAD(enc_angle + delta_angle / 2);
    double dx = delta_disance * cos(mid_angle);
    double dy = delta_disance * sin(mid_angle);
    enc_x += dx;
    enc_y += dy;

    enc_angle = new_angle;
    enc_distance = new_distance;

    chMtxUnlock();

    //attend la prochaine echeance
    chThdSleepUntil(time);
  }
  return 0;
}

void posInit(void)
{
  //initialisation du bus SPI
  palSetPad(GPIOE, GPIOE_SPI4_ENC0_CS);
  palSetPad(GPIOC, GPIOC_SPI4_ENC1_CS);

  //initialisation des variables internes
  chMtxInit(&mutex);
  chMtxLock(&mutex);
  posComputeEncoderPosition(0);
  posComputeEncoderPosition(1);
  posComputeEncoderPosition(0);
  posComputeEncoderPosition(1);
  enc_value[0] = 0;
  enc_value[1] = 0;
  enc_distance = 0;
  enc_angle = 0;
  enc_x = 0;
  enc_y = 0;
  chMtxUnlock();

  //lancement du calcul periodique de la position
  chThdCreateStatic(waPosThread, sizeof(waPosThread), POSITION_SCHEDULER_PRIO, posThread, NULL);
}

double posGetAdeg(void)
{
  chMtxLock(&mutex);
  double a = TICK2DEG(enc_angle);
  chMtxUnlock();

  return a;
}

double posGetXmm(void)
{
  chMtxLock(&mutex);
  double x = TICK2MM(enc_x);
  chMtxUnlock();

  return x;
}

double posGetYmm(void)
{
  chMtxLock(&mutex);
  double y = TICK2DEG(enc_y);
  chMtxUnlock();

  return y;
}
