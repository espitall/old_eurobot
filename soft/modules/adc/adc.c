#include <ch.h>
#include <hal.h>
#include "adc.h"

static const SPIConfig spi5cfg = {
  NULL,
  NULL,
  0,
  ((0x07 << 3) & SPI_CR1_BR) | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_CPOL | 0,
};

static const int spimode = 2;

/**
 * TODO BugsByte :
 * Voir si il faut initialiser autre chose ?
 */
void adcInit(void)
{
  spiStart(&SPID5, &spi5cfg);
}

/**
 * TODO BugsByte :
 * Gérer les 2 séries de canaux
 * de 1 à 8 sur GPIOG_SPI5_ADC0_CS
 * de 9 à 16 sur GPIOG_SPI5_ADC1_CS
 */
uint16_t adcRead (int canal)
{
  //lock SPI bus
  spiAcquireBus(&SPID5);

  //envoi de la commande
  uint8_t raw2send = (1 << 7) | (canal << 3) | (mode << 1);
  
  palClearPad(PORTG, GPIOG_SPI5_ADC0_CS);
  spiSend(&SPID5, 1, &raw2send);
  palSetPad(PORTG, GPIOG_SPI5_ADC0_CS);
  
  //lecture du bus SPI
  uint16_t raw2receive;
  palClearPad(PORTG, GPIOG_SPI5_ADC0_CS);
  spiReceive(&SPID5, 2, &raw2receive);
  palSetPad(PORTG, GPIOG_SPI5_ADC0_CS);

  //libere les ressources
  spiReleaseBus(&SPID5);

  //suppression MSB
  raw2receive = (raw2receive & 0x0FFF);

  return raw2receive;
}

double adcReadMV(int canal)
{
  uint16_t val = adcRead (canal);

  return val * ADC_REFERENCE_VOLTAGE * 1000 / 0x0FFF;
}
