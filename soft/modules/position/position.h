#ifndef POSITION_H
#define POSITION_H

/*
 * Initialisation du module de positionnement
 */
void posInit(void);

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

#endif// POSITION_H
