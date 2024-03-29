/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "ili9341.h"
#include "stm32f429i_discovery_sdram.h"
//#include "stm32_dma2d.h"

#if ILI9341_USE_DRIVER
extern ILI9341Driver ILI9341D1;
#endif

static const SPIConfig spi4cfg = {
  NULL,
  NULL,
  0,
  ((0x07 << 3) & SPI_CR1_BR) | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | 0,
};


static const SPIConfig spi5cfg = {
  NULL,
  NULL,
  0,
  ((0x07 << 3) & SPI_CR1_BR) | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA,
};

static const I2CConfig i2c3cfg = {
  OPMODE_I2C,
  200000,
  FAST_DUTY_CYCLE_2
};


#if HAL_USE_PAL || defined(__DOXYGEN__)
/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
const PALConfig pal_default_config =
{
  {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
   VAL_GPIOA_ODR,   VAL_GPIOA_AFRL,   VAL_GPIOA_AFRH},
  {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
   VAL_GPIOB_ODR,   VAL_GPIOB_AFRL,   VAL_GPIOB_AFRH},
  {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
   VAL_GPIOC_ODR,   VAL_GPIOC_AFRL,   VAL_GPIOC_AFRH},
  {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
   VAL_GPIOD_ODR,   VAL_GPIOD_AFRL,   VAL_GPIOD_AFRH},
  {VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
   VAL_GPIOE_ODR,   VAL_GPIOE_AFRL,   VAL_GPIOE_AFRH},
  {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
   VAL_GPIOF_ODR,   VAL_GPIOF_AFRL,   VAL_GPIOF_AFRH},
  {VAL_GPIOG_MODER, VAL_GPIOG_OTYPER, VAL_GPIOG_OSPEEDR, VAL_GPIOG_PUPDR,
   VAL_GPIOG_ODR,   VAL_GPIOG_AFRL,   VAL_GPIOG_AFRH},
  {VAL_GPIOH_MODER, VAL_GPIOH_OTYPER, VAL_GPIOH_OSPEEDR, VAL_GPIOH_PUPDR,
   VAL_GPIOH_ODR,   VAL_GPIOH_AFRL,   VAL_GPIOH_AFRH},
  {VAL_GPIOI_MODER, VAL_GPIOI_OTYPER, VAL_GPIOI_OSPEEDR, VAL_GPIOI_PUPDR,
   VAL_GPIOI_ODR,   VAL_GPIOI_AFRL,   VAL_GPIOI_AFRH}
};
#endif

/**
 * @brief   Early initialization code.
 * @details This initialization must be performed just after stack setup
 *          and before any other initialization.
 */
void __early_init(void) {

  stm32_clock_init();
}

#if HAL_USE_SDC || defined(__DOXYGEN__)
/**
 * @brief   SDC card detection.
 */
bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {

  (void)sdcp;
  /* TODO: Fill the implementation.*/
  return TRUE;
}

/**
 * @brief   SDC card write protection detection.
 */
bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {

  (void)sdcp;
  /* TODO: Fill the implementation.*/
  return FALSE;
}
#endif /* HAL_USE_SDC */

#if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
/**
 * @brief   MMC_SPI card detection.
 */
bool_t mmc_lld_is_card_inserted(MMCDriver *mmcp) {

  (void)mmcp;
  /* TODO: Fill the implementation.*/
  return TRUE;
}

/**
 * @brief   MMC_SPI card write protection detection.
 */
bool_t mmc_lld_is_write_protected(MMCDriver *mmcp) {

  (void)mmcp;
  /* TODO: Fill the implementation.*/
  return FALSE;
}
#endif

/**
 * @brief   Board-specific initialization code.
 * @todo    Add your board-specific code, if any.
 */

void boardSetCS(unsigned int id)
{
  palClearPad(GPIOG, GPIOG_CS_SEL0);
  palClearPad(GPIOG, GPIOG_CS_SEL1);
  palClearPad(GPIOG, GPIOG_CS_SEL1);
  palClearPad(GPIOC, GPIOC_CS_SEL3);

  if(id & (1 << 0))
  {
    palSetPad(GPIOG, GPIOG_CS_SEL0);
  }
  else
  {
    palClearPad(GPIOG, GPIOG_CS_SEL0);
  }

  if(id & (1 << 1))
  {
    palSetPad(GPIOG, GPIOG_CS_SEL1);
  }
  else
  {
    palClearPad(GPIOG, GPIOG_CS_SEL1);
  }

  if(id & (1 << 2))
  {
    palSetPad(GPIOF, GPIOF_CS_SEL2);
  }
  else
  {
    palClearPad(GPIOF, GPIOF_CS_SEL2);
  }

  if(id & (1 << 3))
  {
    palSetPad(GPIOC, GPIOC_CS_SEL3);
  }
  else
  {
    palClearPad(GPIOC, GPIOC_CS_SEL3);
  }

}

void boardInit(void) 
{
  SDRAM_Init();

#if STM32_LTDC_USE_LTDC
  ltdcInit();
#endif

#if STM32_DMA2D_USE_DMA2D
  dma2dInit();
#endif

#if ILI9341_USE_DRIVER
  ili9341ObjectInit(&ILI9341D1);
#endif
  
  boardSetCS(SPI_CS_NONE);

  //init SPI5 bus
  spiStart(&SPID5, &spi5cfg);

  //init SPI4 bus
  spiStart(&SPID4, &spi4cfg);

  //init I2C3 bus
  i2cStart(&I2CD3, &i2c3cfg);
}
