/*
    ChibiOS/RT

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
#include "clock.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)
/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
const PALConfig pal_default_config =
{
  {VAL_GPIOA_DIR},
  {VAL_GPIOB_DIR},
  {VAL_GPIOC_DIR},
  {VAL_GPIOD_DIR},
  {VAL_GPIOE_DIR}
};
#endif

void __attribute__ ((noinline)) call_reti(void) 
{
  asm volatile("reti");
}

//CH_IRQ_HANDLER(TCD1_OVF_vect) {
ISR (TCD1_OVF_vect, ISR_NAKED){

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
  call_reti();
}

void boardInit(void) 
{
  clock_init();

  //System timer setup
  TCD1.CTRLA = TC_CLKSEL_DIV8_gc;
  TCD1.CTRLB = TC_WGMODE_SS_gc;
  TCD1.PER = 1000;
  TCD1.INTCTRLA = TC_OVFINTLVL_LO_gc;

  //enable interrupt
  PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
  sei();
}
