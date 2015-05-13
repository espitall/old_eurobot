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
void asservSlow(void);
int asservIsEnabled(void);
void asservSetDistanceSetPoint(double set_point);
void asservSetDistanceSetPointSafety(double set_point);
void asservSetAngularSetPoint(double set_point);

int asservGetM0(void);
int asservGetM1(void);
int asservGetAOutput(void);
int asservGetDOutput(void);

#endif// ASSERV_H
