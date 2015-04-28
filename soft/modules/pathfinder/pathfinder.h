#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "../field_2015/field.h"

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
  double f;
  double g;
  double h;
  int parent_x;
  int parent_y;
};

typedef struct pathfinder_map PATHFINDER_MAP;

void pathfinderInit(void);

void pathfinderGotoXYmm(double x, double y);


#endif // PATHFINDER_H
