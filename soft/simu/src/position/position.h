#ifndef POSITION_H
#define POSITION_H

#include "../config.h"

/*
 * La position et l'angle des robots
 */
extern int posRobotGauchePrincipalX,
        posRobotGauchePrincipalY,
        posRobotGauchePrincipalA,
        posRobotGaucheSecondaireX,
        posRobotGaucheSecondaireY,
        posRobotGaucheSecondaireA,
        posRobotDroitePrincipalX,
        posRobotDroitePrincipalY,
        posRobotDroitePrincipalA,
        posRobotDroiteSecondaireX,
        posRobotDroiteSecondaireY,
        posRobotDroiteSecondaireA;

/*
 * La position des balles
 */
extern int posBalle1X,
        posBalle1Y,
        posBalle2X,
        posBalle2Y,
        posBalle3X,
        posBalle3Y,
        posBalle4X,
        posBalle4Y;

/*
 * La position des Gobelets
 */
extern int posGebelet1X,
        posGebelet1Y,
        posGebelet2X,
        posGebelet2Y,
        posGebelet3X,
        posGebelet3Y,
        posGebelet4X,
        posGebelet4Y,
        posGebelet5X,
        posGebelet5Y;

/*
 * La position des plots
 */
extern int posPlotGauche1X,
        posPlotGauche1Y,
        posPlotGauche2X,
        posPlotGauche2Y,
        posPlotGauche3X,
        posPlotGauche3Y,
        posPlotGauche4X,
        posPlotGauche4Y,
        posPlotGauche5X,
        posPlotGauche5Y,
        posPlotGauche6X,
        posPlotGauche6Y,
        posPlotGauche7X,
        posPlotGauche7Y,
        posPlotGauche8X,
        posPlotGauche8Y,
        posPlotDroite1X,
        posPlotDroite1Y,
        posPlotDroite2X,
        posPlotDroite2Y,
        posPlotDroite3X,
        posPlotDroite3Y,
        posPlotDroite4X,
        posPlotDroite4Y,
        posPlotDroite5X,
        posPlotDroite5Y,
        posPlotDroite6X,
        posPlotDroite6Y,
        posPlotDroite7X,
        posPlotDroite7Y,
        posPlotDroite8X,
        posPlotDroite8Y;

/*
 * Coordonnée X en mm
 */
double posGetXmm (void);

/*
 * Coordonnée Y en mm
 */
double posGetYmm (void);

/*
 * Angle en degrés
 */
double posGetAdeg (void);

#endif // POSITION_H
