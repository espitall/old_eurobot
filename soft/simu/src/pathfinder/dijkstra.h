#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../field/field.h"
#include "pathfinder.h"

/*
 * Les différentes listes nécessaire pour l'algorythme
 */
enum dijkstra_listes
{
    DIJKSTRA_NOLISTE,
    DIJKSTRA_OPENLIST,
    DIJKSTRA_CLOSELIST
};

typedef enum dijkstra_listes DIJKSTRA_LISTES;

/*
 * La structure pour l'algo de Dijkstra
 */
struct dijkstra_map_point
{
    DIJKSTRA_LISTES liste;
    double f;
    double g;
    double h;
    int parent_x;
    int parent_y;
};

typedef struct dijkstra_map_point DIJKSTRA_MAP_POINT;

/*
 * Lance l'algo Dijkstra pour atteindre le point end sur la carte à partir du point start
 */
void dijkstra (PATHFINDER_POINT start, PATHFINDER_POINT end);

#endif // DIJKSTRA_H
