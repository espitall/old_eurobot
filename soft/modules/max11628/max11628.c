#include <ch.h>
#include <hal.h>
#include "max11628.h"
#include "lcd.h"

static const SPIConfig spi5cfg = {
  NULL,
  NULL,
  0,
  ((0x07 << 3) & SPI_CR1_BR) | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR,
};

static const int spimode = 3;

/**
 * TODO BugsByte :
 * Voir si il faut initialiser autre chose
 */
void max11628Init(void)
{
  spiStart(&SPID5, &spi5cfg);
}

/**
 * Gére les 2 séries de canaux
 * de 1 à 8 sur GPIOG_SPI5_ADC0_CS
 * de 9 à 16 sur GPIOG_SPI5_ADC1_CS
 */
uint16_t max11628Read (int canal)
{
  canal--;
  int pin;
  if (canal > 8)
  {
    pin = GPIOG_SPI5_ADC1_CS;
    canal -= 8;
  }
  else
    pin = GPIOG_SPI5_ADC0_CS;
  
  //lock SPI bus
  spiAcquireBus(&SPID5);

  //envoi de la commande
  uint8_t raw2send = (1 << 7) | (canal << 3) | (spimode << 1);

  palClearPad(GPIOG, pin);
  spiSend(&SPID5, 1, &raw2send);
  palSetPad(GPIOG, pin);

  // Attente
  chThdSleepMilliseconds(1);

  //réception des données
  uint8_t bufferSend [2] = {0, 0};
  uint8_t bufferReceive [2];

  palClearPad(GPIOG, pin);
  spiExchange (&SPID5, 2, bufferSend, bufferReceive);
  palSetPad(GPIOG, pin);

  //suppression MSB
  uint16_t raw2receive;
  raw2receive = ((bufferReceive [0] & 0xFF) << 8) | (bufferReceive [1] & 0xFF);
  raw2receive = raw2receive & 0x0FFF;

  //libère les ressources
  spiReleaseBus(&SPID5);

  return raw2receive;
}

double max11628ReadmV(int canal)
{
  double val = (double) max11628Read (canal);
  
  return val;
}
