#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

/*
 * Initialisation du module de positionnement
 */
void posInit(void (*position_computed_hook)(void));

/*
 * Coordonn�e X en mm
 */
double posGetXmm(void);
void posSetXmm(double x);

/*
 * Coordonn�e Y en mm
 */
double posGetYmm(void);
void posSetYmm(double y);

/*
 * Angle en degr�s
 */
double posGetAdeg(void);
void posSetAdeg(double a);

/*
 * Distance en mm
 */
double posGetDmm(void);

int32_t posGetRightTick(void);
int32_t posGetLeftTick(void);

void posPrint(void);

#endif// POSITION_H
