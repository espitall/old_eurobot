#ifndef ASSERV_H
#define ASSERV_H

/*
 * Initialisation du module d'asservissement
 */
void asservInit(void);

/*
 * Effectue le calcul d'asservissement
 */
void asservCompute(void);
void asservSetEnable(int enable);
int asservIsEnabled(void);

#endif// ASSERV_H
