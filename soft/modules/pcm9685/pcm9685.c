#include <ch.h>
#include <hal.h>
#include <stdint.h>
#include "pwm.h"
#include "config.h"

#define PCM9685_ADDR 0x70

#define I2C_TIMEOUT_MS 5
#define PCA9685_MODE1_REGISTER 0x00
#define PCA9685_MODE1_REGISTER_SLEEP_bp 4
#define PCA9685_LED0_ON_L_REGISTER 0x06
#define PCA9685_PRE_SCALE_REGISTER 0xFE

static MUTEX_DECL(mutex);

static void pcm9685SetSleep(uint8_t addr, uint8_t sleep)
{
  uint8_t txbuf[2];
  txbuf[0] = PCA9685_MODE1_REGISTER;
  txbuf[1] = (sleep << PCA9685_MODE1_REGISTER_SLEEP_bp) | (1 << 5);

  i2cMasterTransmitTimeout(&I2CD3,addr,txbuf,2,NULL,0,MS2ST(I2C_TIMEOUT_MS));
}

static void pcm9685SetFreq(uint8_t addr, uint32_t freq_Hz)
{
  uint32_t divider = 25000000 / 4096;
  divider /= freq_Hz;
  divider -= 1;

  uint8_t txbuf[2];
  txbuf[0] = PCA9685_PRE_SCALE_REGISTER;
  txbuf[1] = divider;

  i2cMasterTransmitTimeout(&I2CD3,addr,txbuf,2,NULL,0,MS2ST(I2C_TIMEOUT_MS));
}

void pcm9685Init(void)
{
  chMtxInit(&mutex);
  pcm9685SetSleep(PCM9685_ADDR, 1);
  pcm9685SetFreq(PCM9685_ADDR, PCM9685_FREQ_HZ);
  pcm9685SetSleep(PCM9685_ADDR, 0);
}

void pcm9685SetChannel(uint8_t id, uint16_t delay, uint16_t width)
{
  uint8_t txbuf[5];
  txbuf[0] = PCA9685_LED0_ON_L_REGISTER + 4 * (id % 16);
  txbuf[1] = delay & 0xff;
  txbuf[2] = (delay >> 8) & 0xff;
  txbuf[3] = width & 0xff;
  txbuf[4] = (width >> 8) & 0xff;

  chMtxLock(&mutex);
  switch(id / 16)
  {
    case 0:
      i2cMasterTransmitTimeout(&I2CD3,PCM9685_ADDR,txbuf,5,NULL,0,MS2ST(I2C_TIMEOUT_MS));
      break;
  }
  chMtxUnlock();
}
