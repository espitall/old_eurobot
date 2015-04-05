#include <ch.h>
#include <hal.h>
#include "max11628.h"
#include "lcd.h"

static const SPIConfig spi5cfg = {
  NULL,
  NULL,
  0,
  ((0x07 << 3) & SPI_CR1_BR) | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA,
};

static const int spimode = 3;

/**
 * TODO BugsByte :
 * Voir si il faut initialiser autre chose
 */
void max11628Init(void)
{
  uint8_t tx[1];

  boardSetCS(SPI_CS_NONE);
  spiStart(&SPID5, &spi5cfg);

  tx[0] = (1 << 6) | (1 << 5) | (1 << 3);

  spiAcquireBus(&SPID5);

  boardSetCS(SPI_CS_ADC0);
  spiSend(&SPID5, 1, tx);
  boardSetCS(SPI_CS_NONE);

  boardSetCS(SPI_CS_ADC1);
  spiSend(&SPID5, 1, tx);
  boardSetCS(SPI_CS_NONE);


  spiReleaseBus(&SPID5);
}

/**
 * Gére les 2 séries de canaux
 * de 0 à 7 sur GPIOG_SPI5_ADC0_CS
 * de 8 à 15 sur GPIOG_SPI5_ADC1_CS
 */
uint16_t max11628Read (int canal)
{
  int pin;
  if (canal >= 8)
  {
    pin = SPI_CS_ADC1;
    canal -= 8;
  }
  else
    pin = SPI_CS_ADC0;
  
  //lock SPI bus
  spiAcquireBus(&SPID5);

  //envoi de la commande
  uint8_t raw2send = (1 << 7) | (canal << 3) | (spimode << 1);

  boardSetCS(pin);
  spiSend(&SPID5, 1, &raw2send);
  boardSetCS(SPI_CS_NONE);

  // Attente
  chThdSleepMilliseconds(1);

  //réception des données
  uint8_t bufferSend [2] = {0, 0};
  uint8_t bufferReceive [2];

  boardSetCS(pin);
  spiExchange (&SPID5, 2, bufferSend, bufferReceive);
  boardSetCS(SPI_CS_NONE);

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
