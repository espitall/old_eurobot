#ifndef ASTAR_H
#define ASTAR_H

#include "../field/field.h"
#include "pathfinder.h"

/*
 * Les différentes listes nécessaire pour l'algorythme
 */
enum astar_listes
{
    ASTAR_NOLISTE,
    ASTAR_OPENLIST,
    ASTAR_CLOSELIST
};

typedef enum astar_listes ASTAR_LISTES;

/*
 * La structure pour l'algo de l'A-star
 */
struct astar_map_point
{
    ASTAR_LISTES liste;
    double f;
    double g;
    double h;
    int parent_x;
    int parent_y;
};

typedef struct astar_map_point ASTAR_MAP_POINT;

/*
 * Lance l'algo A* pour atteindre les coordonnées [x, y] sur la carte
 */
void astar (PATHFINDER_POINT start, PATHFINDER_POINT end);

#endif // ASTAR_H
