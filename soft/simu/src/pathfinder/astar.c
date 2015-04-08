#include "astar.h"
#include "../field/field.h"
#include "pathfinder.h"
#include "../screen/screen.h"
#include <stdio.h>
#include <stdlib.h>

#define ASTAR_DEBUG

void _astar_addToCloseList (PATHFINDER_POINT point)
{
    astar_map [point.x][point.y].liste = ASTAR_CLOSELIST;

    #ifdef ASTAR_DEBUG
        SCREEN_COLOR couleur = {0xff, 0xff, 0x00};
        screenSetPixel (point.x, point.y,  couleur);
        screenRefresh ();
    #endif
}

void _astar_addToOpenList (PATHFINDER_POINT point)
{
    astar_map [point.x][point.y].liste = ASTAR_OPENLIST;

    #ifdef ASTAR_DEBUG
        SCREEN_COLOR couleur = {0x00, 0xff, 0xff};
        screenSetPixel (point.x, point.y,  couleur);
        screenRefresh ();
    #endif
}

PATHFINDER_POINT _astar_getCurrentNode ()
{
    int i, j;
    int minF = -1;
    PATHFINDER_POINT point;

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            if (astar_map [i][j].liste == ASTAR_OPENLIST)
            {
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

    return point;
}

void _astar_getNeighbours (PATHFINDER_POINT point, PATHFINDER_POINT neighbours [8])
{
    PATHFINDER_POINT point_up = {point.x, point.y - 1, 1};
    PATHFINDER_POINT point_up_left = {point.x - 1, point.y - 1, 1.42};
    PATHFINDER_POINT point_up_right = {point.x + 1, point.y - 1, 1.42};
    PATHFINDER_POINT point_down = {point.x, point.y + 1, 1};
    PATHFINDER_POINT point_down_left = {point.x - 1, point.y + 1, 1.42};
    PATHFINDER_POINT point_down_right = {point.x + 1, point.y + 1, 1.42};
    PATHFINDER_POINT point_left = {point.x - 1, point.y, 1};
    PATHFINDER_POINT point_right = {point.x + 1, point.y, 1};

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
    PATHFINDER_POINT currentNode;
    int i;

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

    while (!_astar_isEmptyOpenList ()) //  stopper la boucle si la liste ouverte est vide
    {
        // a. Récupération du node avec le plus petit F contenu dans la liste ouverte. On le nommera CURRENT.
        currentNode = _astar_getCurrentNode ();

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
            PATHFINDER_POINT node = neighbours [i];

            if (node.x == -1 || node.y == -1)
                continue;

            ASTAR_MAP_POINT point = astar_map [node.x][node.y];

            // Si le node est un obstacle ou est dans la liste fermée ignorez-le et passer à l'analyse d'un autre node.
            if (_astar_isOnCloseList (node) || !fieldIsAccessible (node.x, node.y))
                continue;

            // on calcule le nouveau g
            ASTAR_MAP_POINT parent = astar_map [point.parent_x][point.parent_y];
            int newG = parent.g + node.poids;

            // on calcule le nouveau h
            int newH = (abs (end.y - node.y) + abs (end.x - node.x)) * node.poids;

            // on calcule le nouveau F
            int newF = newH + newG;

            if (_astar_isOnOpenList (node))
            {
                //Si le node est déjà dans la liste ouverte, recalculez son G, s'il est inférieur à l'ancien,
                //faites de CURRENT  son parent(P) et recalculez et enregistrez ses propriétés F et H.
                if (newG < point.g)
                {
                    astar_map [node.x][node.y].parent_x = currentNode.x;
                    astar_map [node.x][node.y].parent_y = currentNode.y;
                    astar_map [node.x][node.y].g = newG;
                    astar_map [node.x][node.y].h = newH;
                    astar_map [node.x][node.y].f = newF;
                }
            }
            else
            {
                //Si le node n'est pas dans la liste ouverte, ajoutez-le à la dite liste et faites de CURRENT son parent(P).
                //Calculez et enregistrez ses propriétés F, G et H.
                _astar_addToOpenList (node);
                astar_map [node.x][node.y].parent_x = currentNode.x;
                astar_map [node.x][node.y].parent_y = currentNode.y;
                astar_map [node.x][node.y].g = newG;
                astar_map [node.x][node.y].h = newH;
                astar_map [node.x][node.y].f = newF;
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

        lastNode.x = point.parent_x;
        lastNode.y = point.parent_y;
    }
}