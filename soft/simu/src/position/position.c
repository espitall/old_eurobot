#include "position.h"

/*
 * La position et l'angle des robots
 */
int posRobotGauchePrincipalX = 70 + ROBOT_PRINCIPAL_PROFONDEUR / 2 + 20;
int posRobotGauchePrincipalY = 1000;
int posRobotGauchePrincipalA = 90;
int posRobotGaucheSecondaireX = 70 + ROBOT_PRINCIPAL_PROFONDEUR + ROBOT_SECONDAIRE_PROFONDEUR / 2 + 40;
int posRobotGaucheSecondaireY = 1000;
int posRobotGaucheSecondaireA = 90;
int posRobotDroitePrincipalX = 3000 - 70 - ROBOT_PRINCIPAL_PROFONDEUR / 2 - 20;
int posRobotDroitePrincipalY = 1000;
int posRobotDroitePrincipalA = -90;
int posRobotDroiteSecondaireX = 3000 - 70 - ROBOT_PRINCIPAL_PROFONDEUR - ROBOT_SECONDAIRE_PROFONDEUR / 2 - 40;
int posRobotDroiteSecondaireY = 1000;
int posRobotDroiteSecondaireA = -90;

/*
 * La position des balles
 */
int posBalle1X = 70 / 2;
int posBalle1Y = 1000;
int posBalle2X = 3000 - (70 / 2);
int posBalle2Y = 1000;
int posBalle3X = 1250;
int posBalle3Y = 2000 - (100 / 2);
int posBalle4X = 1750;
int posBalle4Y = 2000 - (100 / 2);

/*
 * La position des Gobelets
 */
int posGebelet1X = 250;
int posGebelet1Y = 1750;
int posGebelet2X = 910;
int posGebelet2Y = 830;
int posGebelet3X = 1500;
int posGebelet3Y = 1650;
int posGebelet4X = 2090;
int posGebelet4Y = 830;
int posGebelet5X = 2750;
int posGebelet5Y = 1750;

/*
 * La position des plots
 */
int posPlotGauche1X = 90;
int posPlotGauche1Y = 200;
int posPlotGauche2X = 90;
int posPlotGauche2Y = 1750;
int posPlotGauche3X = 90;
int posPlotGauche3Y = 1850;
int posPlotGauche4X = 850;
int posPlotGauche4Y = 100;
int posPlotGauche5X = 850;
int posPlotGauche5Y = 200;
int posPlotGauche6X = 870;
int posPlotGauche6Y = 1355;
int posPlotGauche7X = 1100;
int posPlotGauche7Y = 1770;
int posPlotGauche8X = 1300;
int posPlotGauche8Y = 1400;
int posPlotDroite1X = 3000 - 90;
int posPlotDroite1Y = 200;
int posPlotDroite2X = 3000 - 90;
int posPlotDroite2Y = 1750;
int posPlotDroite3X = 3000 - 90;
int posPlotDroite3Y = 1850;
int posPlotDroite4X = 3000 - 850;
int posPlotDroite4Y = 100;
int posPlotDroite5X = 3000 - 850;
int posPlotDroite5Y = 200;
int posPlotDroite6X = 3000 - 870;
int posPlotDroite6Y = 1355;
int posPlotDroite7X = 3000 - 1100;
int posPlotDroite7Y = 1770;
int posPlotDroite8X = 3000 - 1300;
int posPlotDroite8Y = 1400;

/*
 * Coordonnée X en mm
 */
double posGetXmm(void)
{
    #if ROBOT == PRINCIPAL
        #if EQUIPE == GAUCHE
            return posRobotGauchePrincipalX;
        #else
            return posRobotDroitePrincipalX;
        #endif
    #else
        #if EQUIPE == GAUCHE
            return posRobotGaucheSecondaireX;
        #else
            return posRobotDroiteSecondaireX;
        #endif
    #endif
}

/*
 * Coordonnée Y en mm
 */
double posGetYmm(void)
{
    #if ROBOT == PRINCIPAL
        #if EQUIPE == GAUCHE
            return posRobotGauchePrincipalY;
        #else
            return posRobotDroitePrincipalY;
        #endif
    #else
        #if EQUIPE == GAUCHE
            return posRobotGaucheSecondaireY;
        #else
            return posRobotDroiteSecondaireY;
        #endif
    #endif
}

void posSetXmm(int x)
{
    #if ROBOT == PRINCIPAL
        #if EQUIPE == GAUCHE
            posRobotGauchePrincipalX = x;
        #else
            posRobotDroitePrincipalX = x;
        #endif
    #else
        #if EQUIPE == GAUCHE
            posRobotGaucheSecondaireX = x;
        #else
            posRobotDroiteSecondaireX = x;
        #endif
    #endif
}

void posSetYmm(int y)
{
    #if ROBOT == PRINCIPAL
        #if EQUIPE == GAUCHE
            posRobotGauchePrincipalY = y;
        #else
            posRobotDroitePrincipalY = y;
        #endif
    #else
        #if EQUIPE == GAUCHE
            posRobotGaucheSecondaireY = y;
        #else
            posRobotDroiteSecondaireY = y;
        #endif
    #endif
}

/*
 * Angle en degrés
 */
double posGetAdeg(void)
{
    #if ROBOT == PRINCIPAL
        #if EQUIPE == GAUCHE
            return posRobotGauchePrincipalA;
        #else
            return posRobotDroitePrincipalA;
        #endif
    #else
        #if EQUIPE == GAUCHE
            return posRobotGaucheSecondaireA;
        #else
            return posRobotDroiteSecondaireA;
        #endif
    #endif
}

void posSetAdeg(int a)
{
    #if ROBOT == PRINCIPAL
        #if EQUIPE == GAUCHE
            posRobotGauchePrincipalA = a;
        #else
            posRobotDroitePrincipalA = a;
        #endif
    #else
        #if EQUIPE == GAUCHE
            posRobotGaucheSecondaireA = a;
        #else
            posRobotDroiteSecondaireA = a;
        #endif
    #endif
}