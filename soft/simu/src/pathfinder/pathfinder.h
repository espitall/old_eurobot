#ifndef PATHFINDER_H
#define PATHFINDER_H

/*
 * Structure d'un point
 */
struct pathfinder_point
{
    int x;
    int y;
    double poids;
    double malus;
};

typedef struct pathfinder_point PATHFINDER_POINT;

/*
 * Initialise le pathfinder
 */
void pathfinderInit (void);

/*
 * Lance le pathfinder pour atteindre les coordonnées [x, y] sur la carte
 */
void pathfinderGotoXYmm (double x, double y);

/*
 * Affichage graphique de la carte
 */
void pathfinderMapDesine ();

/*
 * Affichage textuel de la carte
 */
void pathfinderMapDump ();

/*
 * Affichage textuel de la carte
 */
void pathfinderMapDump2 ();

/*
 * Calcul de la distance entre 2 poitns en utilisant une certaine heuristique
 */
double pathfinderHeuristique (double dx, double dy);

/*
 * Calcule l'angle entre 2 points adjacents
 */
double pathfinderAngle (PATHFINDER_POINT parent, PATHFINDER_POINT point);

/*
 * Calcule le delta entre 2 angles
 */
double pathfinderDeltaAngle (double angle1, double angle2);

#endif // PATHFINDER_H
