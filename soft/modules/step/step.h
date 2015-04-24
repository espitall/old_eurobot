#ifndef STEP_H
#define STEP_H

#include <stdint.h>
#include <config.h>

#define STEP_ID 0x4512
#define STEP_STATUS_BUSY 0x0001

typedef enum
{
  STEP_BOARD_ID,
  STEP_COMMAND,
} step_registers_t;

typedef enum
{
  STEP_ACTION_WAIT,
  STEP_ACTION_TAKE_RIGHT,
} step_action_t;

void stepInit(void);

#if defined(STEP_MASTER)

void stepWait(void);
void stepAction(step_action_t action);

#elif defined(STEP_SLAVE)

step_action_t stepGetCommand(void);
void stepSetReady(void);

#else
# error "No master/slave step configuration selected"
#endif

#endif// STEP_H

