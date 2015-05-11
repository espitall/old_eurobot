#ifndef STEPPER_H
#define STEPPER_H

void stepperInit(void);
void stepperSetTarget(int id, int32_t pos);
void stepperSetPosition(int id, int32_t pos);
void stepperWait(int id);
void stepperSetRel(int id, int32_t pos);

#endif// STEPPER_H
