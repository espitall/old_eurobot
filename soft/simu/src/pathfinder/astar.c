#include "astar.h"
#include "../field/field.h"
#include <math.h>
#include "pathfinder.h"
#include "../position/position.h"
#include "../screen/screen.h"
#include <stdio.h>
#include <stdlib.h>

#define ASTAR_DEBUG

void _astar_init ()
{
    int i, j = 0;
    for (i = 0; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            ASTAR_MAP_POINT point;
            point.liste = ASTAR_NOLISTE;
            point.g = 0;
            point.h = 0;
            point.f = 0;
            point.parent_x = -1;
            point.parent_y = -1;
            astar_map [i][j] = point;
        }
    }
}

void _astar_addToCloseList (PATHFINDER_POINT point)
{
    astar_map [point.x][point.y].liste = ASTAR_CLOSELIST;

    #ifdef ASTAR_DEBUG
        SCREEN_COLOR couleur;
        couleur.rouge = 0xff;
        couleur.vert = 0xff;
        couleur.bleu = 0x00;
        screenSetPixel (point.x, point.y,  couleur);
        screenRefresh ();
    #endif
}

void _astar_addToOpenList (PATHFINDER_POINT point)
{
    astar_map [point.x][point.y].liste = ASTAR_OPENLIST;

    #ifdef ASTAR_DEBUG
        SCREEN_COLOR couleur;
        couleur.rouge = 0x00;
        couleur.vert = 0xff;
        couleur.bleu = 0xff;
        screenSetPixel (point.x, point.y,  couleur);
        screenRefresh ();
    #endif
}

PATHFINDER_POINT _astar_getCurrentNode ()
{
    int i, j;
    double minF = -1;
    PATHFINDER_POINT point;

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            if (astar_map [i][j].liste == ASTAR_OPENLIST)
            {
//                printf ("%d %d : %f\n", i, j, astar_map [i][j].f);
                if (minF == -1 || astar_map [i][j].f < minF)
                {
                    minF = astar_map [i][j].f;
                    point.x = i;
                    point.y = j;
                }
            }
        }
    }

    if (minF == -1)
    {
        point.x = -1;
        point.y = -1;
    }

//    printf ("=> %d %d\n", point.x, point.y);

    return point;
}

void _astar_getNeighbours (PATHFINDER_POINT point, PATHFINDER_POINT neighbours [8])
{
    // On récupère l'angle actuel du robot
    ASTAR_MAP_POINT parent = astar_map [point.x][point.y];
    int parent_x = parent.parent_x;
    int parent_y = parent.parent_y;

    double angle;
    if (parent_x == -1 && parent_y == -1)
    {
        // On commence juste le parcours
        // Il faut donc regarder l'angle du robot
        angle = posGetAdeg ();
    }
    else
    {
        PATHFINDER_POINT parentPoint;
        parentPoint.x = parent_x;
        parentPoint.y = parent_y;
        parentPoint.poids = 0;
        parentPoint.malus = 0;
        angle = pathfinderAngle (parentPoint, point);
    }

    printf ("Angle par rapport au noeud précédent : %f\n", angle);

    double distance;
    double delta_angle;

    // Voisin à 0°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, 0);
    PATHFINDER_POINT point_up;
    point_up.x = point.x;
    point_up.y = point.y - 1;
    point_up.poids = distance;
    point_up.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à 45°
    distance = sqrt (2);
    delta_angle = pathfinderDeltaAngle (angle, 45);
    PATHFINDER_POINT point_up_right;
    point_up_right.x = point.x + 1;
    point_up_right.y = point.y - 1;
    point_up_right.poids = distance;
    point_up_right.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à 90°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, 90);
    PATHFINDER_POINT point_right;
    point_right.x = point.x + 1;
    point_right.y = point.y;
    point_right.poids = distance;
    point_right.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à 135°
    distance = sqrt (2);
    delta_angle = pathfinderDeltaAngle (angle, 135);
    PATHFINDER_POINT point_down_right;
    point_down_right.x = point.x + 1;
    point_down_right.y = point.y + 1;
    point_down_right.poids = distance;
    point_down_right.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à 180°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, 180);
    PATHFINDER_POINT point_down;
    point_down.x = point.x;
    point_down.y = point.y + 1;
    point_down.poids = distance;
    point_down.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à -135°
    distance = sqrt (2);
    delta_angle = pathfinderDeltaAngle (angle, -135);
    PATHFINDER_POINT point_down_left;
    point_down_left.x = point.x - 1;
    point_down_left.y = point.y + 1;
    point_down_left.poids = distance;
    point_down_left.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à -90°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, -90);
    PATHFINDER_POINT point_left;
    point_left.x = point.x - 1;
    point_left.y = point.y;
    point_left.poids = distance;
    point_left.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    // Voisin à -45°
    distance = sqrt (2);
    delta_angle = pathfinderDeltaAngle (angle, -45);
    PATHFINDER_POINT point_up_left;
    point_up_left.x = point.x - 1;
    point_up_left.y = point.y - 1;
    point_up_left.poids = distance;
    point_up_left.malus = delta_angle * PATHFINDER_MALUS_ROTATION;

    if (point_up.y < 0)
    {
        point_up.x = -1;
        point_up.y = -1;
    }

    if (point_down.y >= FIELD_Y / FIELD_RESOLUTION)
    {
        point_down.x = -1;
        point_down.y = -1;
    }

    if (point_left.x < 0)
    {
        point_left.x = -1;
        point_left.y = -1;
    }

    if (point_right.x > FIELD_X / FIELD_RESOLUTION)
    {
        point_right.x = -1;
        point_right.y = -1;
    }

    neighbours [0] = point_right;
    neighbours [1] = point_left;
    neighbours [2] = point_down;
    neighbours [3] = point_up;
    neighbours [4] = point_up_left;
    neighbours [5] = point_up_right;
    neighbours [6] = point_down_left;
    neighbours [7] = point_down_right;
}

int _astar_isOnOpenList (PATHFINDER_POINT point)
{
    return (astar_map [point.x][point.y].liste == ASTAR_OPENLIST);
}

int _astar_isOnCloseList (PATHFINDER_POINT point)
{
    return (astar_map [point.x][point.y].liste == ASTAR_CLOSELIST);
}

int _astar_isEmptyOpenList ()
{
    PATHFINDER_POINT currentNode = _astar_getCurrentNode ();
    if (currentNode.x == -1 || currentNode.y == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void astar (PATHFINDER_POINT start, PATHFINDER_POINT end)
{
    _astar_init ();
    PATHFINDER_POINT currentNode;
    int i;

    printf ("Start : [%d, %d]\n", start.x, start.y);
    printf ("End : [%d, %d]\n", end.x, end.y);
    printf ("Distance : %f\n\n", pathfinderHeuristique (abs (end.x - start.x), abs (end.y - start.y)));

    #ifdef ASTAR_DEBUG
        SCREEN_COLOR couleur;

        couleur.rouge = 0xff;
        couleur.vert = 0x00;
        couleur.bleu = 0x00;
        screenSetPixel (start.x, start.y, couleur);
        screenSetPixel (start.x + 1, start.y, couleur);
        screenSetPixel (start.x - 1, start.y, couleur);
        screenSetPixel (start.x, start.y + 1, couleur);
        screenSetPixel (start.x, start.y - 1, couleur);

        couleur.rouge = 0x00;
        couleur.vert = 0xff;
        couleur.bleu = 0x00;
        screenSetPixel (end.x, end.y, couleur);
        screenSetPixel (end.x + 1, end.y, couleur);
        screenSetPixel (end.x - 1, end.y, couleur);
        screenSetPixel (end.x, end.y + 1, couleur);
        screenSetPixel (end.x, end.y - 1, couleur);

        screenRefresh ();
    #endif

    _astar_addToOpenList (start);

    int cpt = 1;
    while (!_astar_isEmptyOpenList ()) //  stopper la boucle si la liste ouverte est vide
    {
        printf ("itération n°%d : \n", cpt);
        // a. Récupération du node avec le plus petit F contenu dans la liste ouverte. On le nommera CURRENT.
        currentNode = _astar_getCurrentNode ();
        printf ("Noeud étudié : [%d, %d]\n", currentNode.x, currentNode.y);

        cpt++;
        if (cpt > 2)
            return;

        //  stopper la boucle si n ajoute le noeud d'arrivée à la liste fermée
        if (currentNode.x == end.x && currentNode.y == end.y)
            break;

        // b. Basculer CURRENT dans la liste fermée.
        _astar_addToCloseList (currentNode);

        //  récupération des voisins de CURRENT
        PATHFINDER_POINT neighbours [8];
        _astar_getNeighbours (currentNode, neighbours);

        // Pour chacun des 8 nodes adjacents à CURRENT appliquer la méthode suivante:
        for (i = 0; i < 8; ++i)
        {
            PATHFINDER_POINT neighbour = neighbours [i];

            if (neighbour.x == -1 || neighbour.y == -1)
                continue;

            // Si le neighbour est un obstacle ou est dans la liste fermée ignorez-le et passer à l'analyse d'un autre neighbour.
            if (_astar_isOnCloseList (neighbour) || !fieldIsAccessible (neighbour.x, neighbour.y))
                continue;


            // on calcule le nouveau g
            double newG = astar_map [currentNode.x][currentNode.y].g + neighbour.poids + neighbour.malus;

            // Si il n'a jamais été analysé ou qu'on lui ait trouvé un meilleur G
            if (astar_map [neighbour.x][neighbour.y].g == 0.0 || newG < astar_map [neighbour.x][neighbour.y].g)
            {
                astar_map [neighbour.x][neighbour.y].g = newG;
                if (astar_map [neighbour.x][neighbour.y].h == 0.0)
                {
                    astar_map [neighbour.x][neighbour.y].h = pathfinderHeuristique (abs (neighbour.x - end.x),
                                                                                    abs (neighbour.y - end.y));
                }
                astar_map [neighbour.x][neighbour.y].f = astar_map [neighbour.x][neighbour.y].g
                                                         + astar_map [neighbour.x][neighbour.y].h;
                astar_map [neighbour.x][neighbour.y].parent_x = currentNode.x;
                astar_map [neighbour.x][neighbour.y].parent_y = currentNode.y;

                _astar_addToOpenList (neighbour);

                printf ("Voisin %d : [%d, %d] (%f + %f = %f)\n",
                        i,
                        neighbour.x,
                        neighbour.y,
                        astar_map [neighbour.x][neighbour.y].g,
                        astar_map [neighbour.x][neighbour.y].h,
                        astar_map [neighbour.x][neighbour.y].f);
            }
        }
    }

    // on est sorti de la liste, on a deux solutions, soit la liste ouverte est vide dans ces cas là il
    // n'y a pas de solutions et on retoure directement finalPath;
    if (_astar_isEmptyOpenList ())
        printf ("Pas de résultat\n");

    // Soit on maintenant on construit le chemin à rebours;
    PATHFINDER_POINT lastNode = end;
    while (lastNode.x != start.x || lastNode.y != start.y)
    {
        ASTAR_MAP_POINT point = astar_map [lastNode.x][lastNode.y];
        printf ("[%d, %d]\n", lastNode.x * FIELD_RESOLUTION, lastNode.y * FIELD_RESOLUTION);

        #ifdef ASTAR_DEBUG
            SCREEN_COLOR couleur;
            couleur.rouge = 0xff;
            couleur.vert = 0x00;
            couleur.bleu = 0xff;
            screenSetPixel (lastNode.x, lastNode.y,  couleur);
            screenRefresh ();
        #endif

        lastNode.x = point.parent_x;
        lastNode.y = point.parent_y;
    }
}