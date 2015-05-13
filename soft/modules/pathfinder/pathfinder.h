#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "../field_2015/field.h"

#define PATHFINDER_MALUS_ROTATION 5
#define PATHFINDER_MALUS_TERRAIN 10
#define PATHFINDER_MALUS_TERRAIN2 5

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
 * La structure pour l'algo de Dijkstra
 */
struct pathfinder_map
{
  FIELD_MAP_ETAT type;
  PATHFINDER_LISTES liste;
  double score;
  PATHFINDER_POINT parent;
};

typedef struct pathfinder_map PATHFINDER_MAP;

void pathfinderInit(void);

int pathfinderGotoXYmm(double x, double y);

#endif // PATHFINDER_H
