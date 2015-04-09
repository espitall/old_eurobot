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
 * Calcul de la distance entre 2 poitns en utilisant une certaine heuristique
 */
double heuristique (double dx, double dy);

#endif // PATHFINDER_H
