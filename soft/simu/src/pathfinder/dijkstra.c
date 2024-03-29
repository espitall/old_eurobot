#include "dijkstra.h"
#include "../field/field.h"
#include <math.h>
#include "pathfinder.h"
#include "../position/position.h"
#include "../screen/screen.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * La carte représentant le terrain
 */
DIJKSTRA_MAP_POINT dijkstra_map [FIELD_X / FIELD_RESOLUTION][FIELD_Y / FIELD_RESOLUTION];

double _dijkstra_angle (PATHFINDER_POINT point)
{
    double angle;

    if (point.x == -1 && point.y == -1)
    {
        angle = posGetAdeg ();
        return angle;
    }

    DIJKSTRA_MAP_POINT parent = dijkstra_map [point.x][point.y];
    int parent_x = parent.parent_x;
    int parent_y = parent.parent_y;

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

    return angle;
}

void _dijkstra_init ()
{
    int i, j = 0;
    for (i = 0; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            DIJKSTRA_MAP_POINT point;
            point.liste = DIJKSTRA_NOLISTE;
            point.g = 0;
            point.h = 0;
            point.f = 0;
            point.parent_x = -1;
            point.parent_y = -1;
            dijkstra_map [i][j] = point;
        }
    }
}

void _dijkstra_addToCloseList (PATHFINDER_POINT point)
{
    dijkstra_map [point.x][point.y].liste = DIJKSTRA_CLOSELIST;

    #ifdef DIJKSTRA_DEBUG
    SCREEN_COLOR couleur;
    couleur.rouge = 0xff;
    couleur.vert = 0xff;
    couleur.bleu = 0x00;
    screenSetPixel (point.x, point.y,  couleur);
    screenRefresh ();
    #endif
}

void _dijkstra_addToOpenList (PATHFINDER_POINT point)
{
    dijkstra_map [point.x][point.y].liste = DIJKSTRA_OPENLIST;

    #ifdef DIJKSTRA_DEBUG
    SCREEN_COLOR couleur;
    couleur.rouge = 0x00;
    couleur.vert = 0xff;
    couleur.bleu = 0xff;
    screenSetPixel (point.x, point.y,  couleur);
    screenRefresh ();
    #endif
}

PATHFINDER_POINT _dijkstra_getCurrentNode (PATHFINDER_POINT currentNode)
{
    int i, j;
    double minF = -1;
    double minDistance = -1;
    double distance;
    double angle2;
    int memeAngle = 0;
    PATHFINDER_POINT point, point2;

    // On récupère l'angle actuel du robot
    double angle = _dijkstra_angle (currentNode);

    for (i = 0; i < FIELD_X / FIELD_RESOLUTION ; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            if (dijkstra_map [i][j].liste == DIJKSTRA_OPENLIST)
            {
                if (minF == -1 || dijkstra_map [i][j].f <= minF)
                {
                    if (dijkstra_map [i][j].f == minF)
                    {
                        distance = 0/*pathfinderHeuristique (abs (currentNode.x - i), abs (currentNode.y - j))*/;
                        if (minDistance == -1 || distance <= minDistance)
                        {
                            if (memeAngle == 0)
                            {
                                minF = dijkstra_map [i][j].f;
                                point.x = i;
                                point.y = j;
                                minDistance = distance;

                                point2.x = i;
                                point2.y = j;
                                angle2 = pathfinderAngle (currentNode, point2);

                                if (angle == angle2)
                                {
                                    //printf ("%d %d, %d %d\n", currentNode.x, currentNode.y, point2.x, point2.y);
                                    memeAngle = 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        minF = dijkstra_map [i][j].f;
                        point.x = i;
                        point.y = j;
                        minDistance = -1;
                        memeAngle = 0;
                    }
                }
            }
        }
    }

    if (minF == -1)
    {
        point.x = -1;
        point.y = -1;
    }

    return point;
}

#if PATHFINDER_WITH_DIAGONALE == TRUE
void _dijkstra_getNeighbours (PATHFINDER_POINT point, PATHFINDER_POINT neighbours [8])
#else
void _dijkstra_getNeighbours (PATHFINDER_POINT point, PATHFINDER_POINT neighbours [4])
#endif
{
    // On récupère l'angle actuel du robot
    double angle = _dijkstra_angle (point);

    #ifdef DIJKSTRA_DEBUG2
    DIJKSTRA_MAP_POINT parent = dijkstra_map [point.x][point.y];
    int parent_x = parent.parent_x;
    int parent_y = parent.parent_y;

    printf ("Angle par rapport au noeud précédent [%d, %d] : %f\n", parent_x, parent_y, angle);
    #endif

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
    if (point_up.y < 0)
    {
        point_up.x = -1;
        point_up.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin 0° : [%d, %d] deltaAngle = %f\n", point_up.x, point_up.y, delta_angle);
    #endif

    #if PATHFINDER_WITH_DIAGONALE == TRUE
    // Voisin à 45°
    distance = PATHFINDER_COUT_DIAGONALE;
    delta_angle = pathfinderDeltaAngle (angle, 45);
    PATHFINDER_POINT point_up_right;
    point_up_right.x = point.x + 1;
    point_up_right.y = point.y - 1;
    point_up_right.poids = distance;
    point_up_right.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_up_right.y < 0)
    {
        point_up_right.x = -1;
        point_up_right.y = -1;
    }
    if (point_up_right.x > FIELD_X / FIELD_RESOLUTION)
    {
        point_up_right.x = -1;
        point_up_right.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin 45° : [%d, %d] deltaAngle = %f\n", point_up_right.x, point_up_right.y, delta_angle);
    #endif
    #endif

    // Voisin à 90°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, 90);
    PATHFINDER_POINT point_right;
    point_right.x = point.x + 1;
    point_right.y = point.y;
    point_right.poids = distance;
    point_right.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_right.x > FIELD_X / FIELD_RESOLUTION)
    {
        point_right.x = -1;
        point_right.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin 90° : [%d, %d] deltaAngle = %f\n", point_right.x, point_right.y, delta_angle);
    #endif

    #if PATHFINDER_WITH_DIAGONALE == TRUE
    // Voisin à 135°
    distance = PATHFINDER_COUT_DIAGONALE;
    delta_angle = pathfinderDeltaAngle (angle, 135);
    PATHFINDER_POINT point_down_right;
    point_down_right.x = point.x + 1;
    point_down_right.y = point.y + 1;
    point_down_right.poids = distance;
    point_down_right.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_down_right.y >= FIELD_Y / FIELD_RESOLUTION)
    {
        point_down_right.x = -1;
        point_down_right.y = -1;
    }
    if (point_down_right.x > FIELD_X / FIELD_RESOLUTION)
    {
        point_down_right.x = -1;
        point_down_right.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin 135° : [%d, %d] deltaAngle = %f\n", point_down_right.x, point_down_right.y, delta_angle);
    #endif
    #endif

    // Voisin à 180°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, 180);
    PATHFINDER_POINT point_down;
    point_down.x = point.x;
    point_down.y = point.y + 1;
    point_down.poids = distance;
    point_down.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_down.y >= FIELD_Y / FIELD_RESOLUTION)
    {
        point_down.x = -1;
        point_down.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin 180° : [%d, %d] deltaAngle = %f\n", point_down.x, point_down.y, delta_angle);
    #endif

    #if PATHFINDER_WITH_DIAGONALE == TRUE
    // Voisin à -135°
    distance = PATHFINDER_COUT_DIAGONALE;
    delta_angle = pathfinderDeltaAngle (angle, -135);
    PATHFINDER_POINT point_down_left;
    point_down_left.x = point.x - 1;
    point_down_left.y = point.y + 1;
    point_down_left.poids = distance;
    point_down_left.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_down_left.y >= FIELD_Y / FIELD_RESOLUTION)
    {
        point_down_left.x = -1;
        point_down_left.y = -1;
    }
    if (point_down_left.x < 0)
    {
        point_down_left.x = -1;
        point_down_left.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin -135° : [%d, %d] deltaAngle = %f\n", point_down_left.x, point_down_left.y, delta_angle);
    #endif
    #endif

    // Voisin à -90°
    distance = 1;
    delta_angle = pathfinderDeltaAngle (angle, -90);
    PATHFINDER_POINT point_left;
    point_left.x = point.x - 1;
    point_left.y = point.y;
    point_left.poids = distance;
    point_left.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_left.x < 0)
    {
        point_left.x = -1;
        point_left.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin -90° : [%d, %d] deltaAngle = %f\n", point_left.x, point_left.y, delta_angle);
    #endif

    #if PATHFINDER_WITH_DIAGONALE == TRUE
    // Voisin à -45°
    distance = PATHFINDER_COUT_DIAGONALE;
    delta_angle = pathfinderDeltaAngle (angle, -45);
    PATHFINDER_POINT point_up_left;
    point_up_left.x = point.x - 1;
    point_up_left.y = point.y - 1;
    point_up_left.poids = distance;
    point_up_left.malus = delta_angle * PATHFINDER_MALUS_ROTATION;
    if (point_up_left.y < 0)
    {
        point_up_left.x = -1;
        point_up_left.y = -1;
    }
    if (point_up_left.x < 0)
    {
        point_up_left.x = -1;
        point_up_left.y = -1;
    }
    #ifdef DIJKSTRA_DEBUG2
    printf ("Voisin -45° : [%d, %d] deltaAngle = %f\n", point_up_left.x, point_up_left.y, delta_angle);
    #endif
    #endif

    neighbours [0] = point_up;
    neighbours [1] = point_right;
    neighbours [2] = point_down;
    neighbours [3] = point_left;
    #if PATHFINDER_WITH_DIAGONALE == TRUE
    neighbours [4] = point_up_right;
    neighbours [5] = point_down_right;
    neighbours [6] = point_down_left;
    neighbours [7] = point_up_left;
    #endif
}

int _dijkstra_isOnOpenList (PATHFINDER_POINT point)
{
    return (dijkstra_map [point.x][point.y].liste == DIJKSTRA_OPENLIST);
}

int _dijkstra_isOnCloseList (PATHFINDER_POINT point)
{
    return (dijkstra_map [point.x][point.y].liste == DIJKSTRA_CLOSELIST);
}

int _dijkstra_isEmptyOpenList ()
{
    int i, j;

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            if (dijkstra_map [i][j].liste == DIJKSTRA_OPENLIST)
                return 0;
        }
    }

    return 1;
}

void dijkstra (PATHFINDER_POINT start, PATHFINDER_POINT end)
{
    _dijkstra_init ();
    PATHFINDER_POINT currentNode;
    int i;

    currentNode.x = -1;
    currentNode.y = -1;
    currentNode.poids = 0;
    currentNode.malus = 0;

    #ifdef DIJKSTRA_DEBUG2
    printf ("Start : [%d, %d]\n", start.x, start.y);
    printf ("End : [%d, %d]\n", end.x, end.y);
    printf ("Distance : %f\n\n", (double) 0/*pathfinderHeuristique (abs (end.x - start.x), abs (end.y - start.y))*/);
    #endif

    #ifdef DIJKSTRA_DEBUG
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

    _dijkstra_addToOpenList (start);

    int cpt = 1;
    while (!_dijkstra_isEmptyOpenList ()) //  stopper la boucle si la liste ouverte est vide
    {
        #ifdef DIJKSTRA_DEBUG2
        printf ("\nItération n°%d : \n", cpt);
        #endif
        // a. Récupération du node avec le plus petit F contenu dans la liste ouverte. On le nommera CURRENT.
        currentNode = _dijkstra_getCurrentNode (currentNode);
        #ifdef DIJKSTRA_DEBUG2
        printf ("Noeud étudié : [%d, %d] (f = %f)\n", currentNode.x, currentNode.y, dijkstra_map [currentNode.x][currentNode.y].f);
        #endif

        cpt++;
//        if (cpt > 800 - 126 + 100)
//            return;

        //  stopper la boucle si on ajoute le noeud d'arrivée à la liste fermée
        if (currentNode.x == end.x && currentNode.y == end.y)
            break;

        // b. Basculer CURRENT dans la liste fermée.
        _dijkstra_addToCloseList (currentNode);

        //  récupération des voisins de CURRENT
        #if PATHFINDER_WITH_DIAGONALE == TRUE
        PATHFINDER_POINT neighbours [8];
        #else
        PATHFINDER_POINT neighbours [4];
        #endif
        _dijkstra_getNeighbours (currentNode, neighbours);

        #ifdef DIJKSTRA_DEBUG2
        printf ("Seuls les voisins suivants reste à étudier :\n");
        #endif

        // Pour chacun des 8 nodes adjacents à CURRENT appliquer la méthode suivante:
        #if PATHFINDER_WITH_DIAGONALE == TRUE
        for (i = 0; i < 8; ++i)
        #else
        for (i = 0; i < 4; ++i)
        #endif
        {
            PATHFINDER_POINT neighbour = neighbours [i];

            #ifdef DIJKSTRA_DEBUG2
            printf ("Voisin %d : [%d, %d]\n",
                    i,
                    neighbour.x,
                    neighbour.y);
            #endif

            if (neighbour.x == -1 || neighbour.y == -1)
                continue;

            // Si le neighbour est un obstacle ou est dans la liste fermée ignorez-le et passer à l'analyse d'un autre neighbour.
            if (_dijkstra_isOnCloseList (neighbour) || !fieldIsAccessible (neighbour.x, neighbour.y))
                continue;

            #ifdef DIJKSTRA_DEBUG2
            printf ("Voisin %d : [%d, %d] (%f + %f = %f)\n",
                    i,
                    neighbour.x,
                    neighbour.y,
                    dijkstra_map [neighbour.x][neighbour.y].g,
                    dijkstra_map [neighbour.x][neighbour.y].h,
                    dijkstra_map [neighbour.x][neighbour.y].f);
            #endif

            // on calcule le nouveau g
            int malus_terrain = fieldMalusTerrain (neighbour.x, neighbour.y);
            double newG = dijkstra_map [currentNode.x][currentNode.y].g + neighbour.poids + neighbour.malus + malus_terrain;
            #ifdef DIJKSTRA_DEBUG2
            printf ("newG = %f\n", newG);
            #endif

            // Si il n'a jamais été analysé ou qu'on lui ait trouvé un meilleur G
            if (dijkstra_map [neighbour.x][neighbour.y].g == 0.0 || newG <= dijkstra_map [neighbour.x][neighbour.y].g)
            {
                dijkstra_map [neighbour.x][neighbour.y].g = newG;
                if (dijkstra_map [neighbour.x][neighbour.y].h == 0.0)
                {
                    dijkstra_map [neighbour.x][neighbour.y].h = 0/*pathfinderHeuristique (abs (neighbour.x - end.x),
                                                                                    abs (neighbour.y - end.y))*/;
                }
                dijkstra_map [neighbour.x][neighbour.y].f = dijkstra_map [neighbour.x][neighbour.y].g
                                                         + dijkstra_map [neighbour.x][neighbour.y].h;
                dijkstra_map [neighbour.x][neighbour.y].parent_x = currentNode.x;
                dijkstra_map [neighbour.x][neighbour.y].parent_y = currentNode.y;

                _dijkstra_addToOpenList (neighbour);

                #ifdef DIJKSTRA_DEBUG2
                printf ("Ajout ou modif voisin %d : [%d, %d] (%f + %f = %f)\n",
                        i,
                        neighbour.x,
                        neighbour.y,
                        dijkstra_map [neighbour.x][neighbour.y].g,
                        dijkstra_map [neighbour.x][neighbour.y].h,
                        dijkstra_map [neighbour.x][neighbour.y].f);
                #endif
            }
        }
    }

    // on est sorti de la liste, on a deux solutions, soit la liste ouverte est vide dans ces cas là il
    // n'y a pas de solutions et on retoure directement finalPath;
    if (_dijkstra_isEmptyOpenList ())
        printf ("Pas de résultat\n");

    // Soit on maintenant on construit le chemin à rebours;
    PATHFINDER_POINT lastNode = end;
    double angle = -1;
    double angle2;
    while (lastNode.x != start.x || lastNode.y != start.y)
    {
        PATHFINDER_POINT point1, point2;

        point1.x = lastNode.x;
        point1.y = lastNode.y;

        DIJKSTRA_MAP_POINT point = dijkstra_map [lastNode.x][lastNode.y];
        #ifdef DIJKSTRA_DEBUG2
        printf ("[%d, %d]\n", lastNode.x * FIELD_RESOLUTION, lastNode.y * FIELD_RESOLUTION);
        #endif

        #ifdef DIJKSTRA_DEBUG
        SCREEN_COLOR couleur;
        couleur.rouge = 0xff;
        couleur.vert = 0x00;
        couleur.bleu = 0xff;
        screenSetPixel (lastNode.x, lastNode.y, couleur);
        screenRefresh ();
        #endif

        lastNode.x = point.parent_x;
        lastNode.y = point.parent_y;

        point2.x = lastNode.x;
        point2.y = lastNode.y;

        angle2 = pathfinderAngle (point1, point2);

        if (angle == -1 || angle != angle2)
        {
            printf ("[%d, %d]\n", point1.x * FIELD_RESOLUTION, point1.y * FIELD_RESOLUTION);
            angle = angle2;
        }
    }
    printf ("Checkpoint [%d, %d]\n", start.x * FIELD_RESOLUTION, start.y * FIELD_RESOLUTION);
}