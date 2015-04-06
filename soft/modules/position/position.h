#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

/*
 * Initialisation du module de positionnement
 */
void posInit(void (*position_computed_hook)(void));

/*
 * Coordonnée X en mm
 */
double posGetXmm(void);
void posSetXmm(double x);

/*
 * Coordonnée Y en mm
 */
double posGetYmm(void);
void posSetYmm(double y);

/*
 * Angle en degrés
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
