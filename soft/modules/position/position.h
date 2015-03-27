#ifndef POSITION_H
#define POSITION_H

/*
 * Initialisation du module de positionnement
 */
void posInit(void (*position_computed_hook)(void));

/*
 * Coordonn�e X en mm
 */
double posGetXmm(void);

/*
 * Coordonn�e Y en mm
 */
double posGetYmm(void);

/*
 * Angle en degr�s
 */
double posGetAdeg(void);

/*
 * Distance en mm
 */
double posGetDmm(void);

void posPrint(void);

#endif// POSITION_H
