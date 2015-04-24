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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F429I-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_STEP_2015
#define BOARD_NAME                  "Eurobot 2015 - Step"


/*
 * IO pins assignments.
 */
#define GPIOA_ENABLE                0
#define GPIOA_RESET                 1
#define GPIOA_SLEEP                 2
#define GPIOA_LED                   3
#define GPIOA_SWITCH0               4
#define GPIOA_SWITCH1               5
#define GPIOA_SWITCH2               6
#define GPIOA_SWITCH3               7

#define GPIOB_MS1                   0
#define GPIOB_MS2                   1
#define GPIOB_MS3                   2
#define GPIOB_PIN3                  3
#define GPIOB_PIN4                  4
#define GPIOB_PIN5                  5
#define GPIOB_PIN6                  6
#define GPIOB_PIN7                  7

#define GPIOC_M0_DIR                0
#define GPIOC_M0_STP                1
#define GPIOC_M1_DIR                2
#define GPIOC_M1_STP                3
#define GPIOC_M2_DIR                4
#define GPIOC_M2_STP                5
#define GPIOC_M3_DIR                6
#define GPIOC_M3_STP                7

#define GPIOD_SERVO0                0
#define GPIOD_SERVO1                1
#define GPIOD_PIN2                  2
#define GPIOD_PIN3                  3
#define GPIOD_SPI_CS                4
#define GPIOD_SPI_MOSI              5
#define GPIOD_SPI_MISO              6
#define GPIOD_SPI_SCK               7

#define GPIOE_PIN0                  0
#define GPIOE_PIN1                  1
#define GPIOE_PIN2                  2
#define GPIOE_PIN3                  3
#define GPIOE_PIN4                  4
#define GPIOE_PIN5                  5
#define GPIOE_PIN6                  6
#define GPIOE_PIN7                  7


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 */

#define PIN_MODE_INPUT(n)           (0U << (n))
#define PIN_MODE_OUTPUT(n)          (1U << (n))

#define VAL_GPIOA_DIR              (PIN_MODE_OUTPUT(GPIOA_ENABLE) |         \
                                    PIN_MODE_OUTPUT(GPIOA_RESET) |          \
                                    PIN_MODE_OUTPUT(GPIOA_SLEEP) |          \
                                    PIN_MODE_OUTPUT(GPIOA_LED) |            \
                                    PIN_MODE_INPUT(GPIOA_SWITCH0) |         \
                                    PIN_MODE_INPUT(GPIOA_SWITCH1) |         \
                                    PIN_MODE_INPUT(GPIOA_SWITCH2) |         \
                                    PIN_MODE_INPUT(GPIOA_SWITCH3))

#define VAL_GPIOB_DIR              (PIN_MODE_OUTPUT(GPIOB_MS1) |             \
                                    PIN_MODE_OUTPUT(GPIOB_MS2)    |          \
                                    PIN_MODE_OUTPUT(GPIOB_MS3)    |          \
                                    PIN_MODE_INPUT(GPIOB_PIN3)    |         \
                                    PIN_MODE_INPUT(GPIOB_PIN4)    |         \
                                    PIN_MODE_INPUT(GPIOB_PIN5)    |         \
                                    PIN_MODE_INPUT(GPIOB_PIN6)    |         \
                                    PIN_MODE_INPUT(GPIOB_PIN7))

#define VAL_GPIOC_DIR              (PIN_MODE_OUTPUT(GPIOC_M0_DIR) |         \
                                    PIN_MODE_OUTPUT(GPIOC_M0_STP) |         \
                                    PIN_MODE_OUTPUT(GPIOC_M1_DIR)  |        \
                                    PIN_MODE_OUTPUT(GPIOC_M1_STP)  |        \
                                    PIN_MODE_OUTPUT(GPIOC_M2_DIR) |         \
                                    PIN_MODE_OUTPUT(GPIOC_M2_STP) |         \
                                    PIN_MODE_OUTPUT(GPIOC_M3_DIR)  |        \
                                    PIN_MODE_OUTPUT(GPIOC_M3_STP))

#define VAL_GPIOD_DIR              (PIN_MODE_OUTPUT(GPIOD_SERVO0)      |    \
                                    PIN_MODE_OUTPUT(GPIOD_SERVO1)      |    \
                                    PIN_MODE_INPUT(GPIOD_PIN2)    |         \
                                    PIN_MODE_INPUT(GPIOD_PIN3)   |          \
                                    PIN_MODE_INPUT(GPIOD_SPI_CS)    |       \
                                    PIN_MODE_INPUT(GPIOD_SPI_MOSI)  |       \
                                    PIN_MODE_INPUT(GPIOD_SPI_MISO)  |       \
                                    PIN_MODE_INPUT(GPIOD_SPI_SCK))

#define VAL_GPIOE_DIR              (PIN_MODE_INPUT(GPIOE_PIN0) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN1) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN2) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN3) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN4) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN5) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN6) |           \
                                    PIN_MODE_INPUT(GPIOE_PIN7))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */

