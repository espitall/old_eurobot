/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    AVR/chcore.c
 * @brief   AVR architecture port code.
 *
 * @addtogroup AVR_CORE
 * @{
 */

#include "ch.h"

volatile int xmega_isr_flag;

void _port_reti(void) {
  asm volatile ("reti");
}

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 * @note    The function is declared as a weak symbol, it is possible to
 *          redefine it in your application code.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if !defined(__DOXYGEN__)
//__attribute__((naked, weak))
#endif
void port_switch(Thread *ntp, Thread *otp) {
  (void)ntp;
  (void)otp;

  asm volatile (
                "push    r0\n\t"
                "in    r0, __SREG__\n\t"
                "push    r0\n\t"
                "push    r1\n\t"
                "push    r2\n\t"
                "push    r3\n\t"
                "push    r4\n\t"
                "push    r5\n\t"
                "push    r6\n\t"
                "push    r7\n\t"
                "push    r8\n\t"
                "push    r9\n\t"
                "push    r10\n\t"
                "push    r11\n\t"
                "push    r12\n\t"
                "push    r13\n\t"
                "push    r14\n\t"
                "push    r15\n\t"
                "push    r16\n\t"
                "push    r17\n\t"
                "push    r18\n\t"
                "push    r19\n\t"
                "push    r20\n\t"
                "push    r21\n\t"
                "push    r22\n\t"
                "push    r23\n\t"
                "push    r24\n\t"
                "push    r25\n\t"
                "push    r26\n\t"
                "push    r27\n\t"
                "push    r28\n\t"
                "push    r29\n\t"
                "push    r30\n\t"
                "push    r31\n\t"
                "in    r16, 0x38\n\t"
                "push    r16\n\t"
                "in    r16, 0x39\n\t"
                "push    r16\n\t"
                "in    r16, 0x3a\n\t"
                "push    r16\n\t"
                "in    r16, 0x3b\n\t"
                "push    r16\n\t"
                "in    r16, 0x3c\n\t"
                "push    r16\n\t"

                "movw    r30, r22\n\t"
                "in    r0, 0x3d\n\t"
                "std    Z+5, r0\n\t"
                "in    r0, 0x3e\n\t"
                "std    Z+6, r0\n\t"

                "movw    r30, r24\n\t"
                "ldd    r0, Z+5\n\t"
                "out    0x3d, r0\n\t"
                "ldd    r0, Z+6\n\t"
                "out    0x3e, r0\n\t"

                "pop    r16\n\t"
                "out    0x3c, r16\n\t"
                "pop    r16\n\t"
                "out    0x3b, r16\n\t"
                "pop    r16\n\t"
                "out    0x3a, r16\n\t"
                "pop    r16\n\t"
                "out    0x39, r16\n\t"
                "pop    r16\n\t"
                "out    0x38, r16\n\t"
                "pop    r31\n\t"
                "pop    r30\n\t"
                "pop    r29\n\t"
                "pop    r28\n\t"
                "pop    r27\n\t"
                "pop    r26\n\t"
                "pop    r25\n\t"
                "pop    r24\n\t"
                "pop    r23\n\t"
                "pop    r22\n\t"
                "pop    r21\n\t"
                "pop    r20\n\t"
                "pop    r19\n\t"
                "pop    r18\n\t"
                "pop    r17\n\t"
                "pop    r16\n\t"
                "pop    r15\n\t"
                "pop    r14\n\t"
                "pop    r13\n\t"
                "pop    r12\n\t"
                "pop    r11\n\t"
                "pop    r10\n\t"
                "pop    r9\n\t"
                "pop    r8\n\t"
                "pop    r7\n\t"
                "pop    r6\n\t"
                "pop    r5\n\t"
                "pop    r4\n\t"
                "pop    r3\n\t"
                "pop    r2\n\t"
                "pop    r1\n\t"
                "pop    r0\n\t"
                "out    __SREG__, r0\n\t"
                "pop    r0\n\t"
                "ret\n\t"
                );
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected (for example because a programming
 *          error in the application code that triggers an assertion while in
 *          debug mode).
 * @note    The function is declared as a weak symbol, it is possible to
 *          redefine it in your application code.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak))
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {

  chSysUnlock();
  asm volatile ("movw    r24, r4");
  asm volatile ("movw    r30, r2");
  asm volatile ("icall");
  asm volatile ("call    chThdExit");
}

/** @} */
