#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../field/field.h"
#include "pathfinder.h"

/*
 * La structure pour l'algo de Dijkstra
 */
struct dijkstra_map_point
{
};

typedef struct dijkstra_map_point DIJKSTRA_MAP_POINT;

/*
 * La carte représentant le terrain
 */
DIJKSTRA_MAP_POINT dijkstra_map [FIELD_X / FIELD_RESOLUTION][FIELD_Y / FIELD_RESOLUTION];

/*
 * Lance l'algo A* pour atteindre les coordonnées [x, y] sur la carte
 */
void dijkstra (PATHFINDER_POINT start, PATHFINDER_POINT end);

#endif // DIJKSTRA_H
