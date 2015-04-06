#ifndef PATHFINDER_H
#define PATHFINDER_H

/*
 * Les différentes listes nécessaire pour l'algorythme
 */
enum pathfinder_listes
{
    PATHFINDER_NOLISTE,
    PATHFINDER_OPENLIST,
    PATHFINDER_CLOSELIST
};

typedef enum pathfinder_listes PATHFINDER_LISTES;

/*
 * Structure d'un point
 */
struct pathfinder_point
{
    int x;
    int y;
    int poids;
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

#endif // PATHFINDER_H
