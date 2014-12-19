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

static const int spimode = 2;

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
  
  chThdSleepMilliseconds(1);
  
  //lecture du bus SPI
  uint8_t buffer [2];
  uint16_t raw2receive;
  palClearPad(GPIOG, pin);
  spiReceive(&SPID5, 2, &buffer);
  palSetPad(GPIOG, pin);

  //libère les ressources
  spiReleaseBus(&SPID5);

  //suppression MSB
  raw2receive = ((buffer [0] & 0xFF) << 8) | (buffer [1] & 0xFF);
  raw2receive = raw2receive & 0x0FFF;
    //raw2receive = ((buffer [1] & 0xFF) << 8) | (buffer [0] & 0xFF);

  return raw2receive;
}

double max11628ReadMV(int canal)
{
  uint32_t val = max11628Read (canal);
  double mv = val * MAX11628_REFERENCE_VOLTAGE * 1000 / 0x0FFF;

  lcdPrintln("%lu => %d", val, (int) mv);
  
  return mv;
}
