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

/**
 * @file    XAVR/pal_lld.h
 * @brief   Xmega low level driver header.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief   GPIO port setup info.
 */
typedef struct {
  /** Initial value for DIR register.*/
  uint32_t      dir;
} xavr_gpio_setup_t;

/**
 * @brief   xavr GPIO static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialize the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
  /** @brief Port A setup data.*/
  xavr_gpio_setup_t    PAData;
  /** @brief Port B setup data.*/
  xavr_gpio_setup_t    PBData;
  /** @brief Port C setup data.*/
  xavr_gpio_setup_t    PCData;
  /** @brief Port D setup data.*/
  xavr_gpio_setup_t    PDData;
  /** @brief Port E setup data.*/
  xavr_gpio_setup_t    PEData;
} PALConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 8


/**
 * @brief Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Digital I/O modes.
 */
typedef uint32_t iomode_t;

/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef PORT_t * ioportid_t;

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   GPIO ports subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init(config) _pal_lld_init(config)

/**
 * @brief   Reads an I/O port.
 * @details This function is implemented by reading the GPIO IDR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((port)->IN)


/**
 * @brief   Writes on a I/O port.
 * @details This function is implemented by writing the GPIO ODR register, the
 *          implementation has no side effects.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) ((port)->OUT = (bits))

/**
 * @brief   Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @notapi
 */
#define pal_lld_setport(port, bits) ((port)->OUTSET = (bits))

/**
 * @brief   Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BRR register, the
 *          implementation has no side effects.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @notapi
 */
#define pal_lld_clearport(port, bits) ((port)->OUTCLR = (bits))

#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE

#define GPIOA (&PORTA)
#define GPIOB (&PORTB)
#define GPIOC (&PORTC)
#define GPIOD (&PORTD)
#define GPIOE (&PORTE)

extern const PALConfig pal_default_config;

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(const PALConfig *config);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */

