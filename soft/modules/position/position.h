#ifndef POSITION_H
#define POSITION_H

/*
 * Initialisation du module de positionnement
 */
void posInit(void (*position_computed_hook)(void));

/*
 * Coordonnée X en mm
 */
double posGetXmm(void);

/*
 * Coordonnée Y en mm
 */
double posGetYmm(void);

/*
 * Angle en degrés
 */
double posGetAdeg(void);

/*
 * Distance en mm
 */
double posGetDmm(void);

void posPrint(void);

#endif// POSITION_H
