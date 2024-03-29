#include "../config.h"
#if PATHFINDER_ALGO == ASTAR
    #include "astar.h"
#elif PATHFINDER_ALGO == DIJKSTRA
    #include "dijkstra.h"
#endif
#include "../field/field.h"
#include <math.h>
#include "pathfinder.h"
#include "../position/position.h"
#include "../screen/screen.h"
#include <stdio.h>

/*
 * Initialise le pathfinder
 */
void pathfinderInit (void)
{
}

/*
 * Lance le pathfinder pour atteindre les coordonnées [x, y] sur la carte
 */
void pathfinderGotoXYmm (double x, double y)
{
    PATHFINDER_POINT start;
    start.x = posGetXmm () / FIELD_RESOLUTION;
    start.y = posGetYmm () / FIELD_RESOLUTION;
    start.poids = 0;
    start.malus = 0;

    PATHFINDER_POINT end;
    end.x = x / FIELD_RESOLUTION;
    end.y = y / FIELD_RESOLUTION;
    end.poids = 0;
    end.malus = 0;

    printf ("%d %d => %d %d\n", (int) posGetXmm (), (int) posGetYmm (), (int) x, (int) y);

    if (fieldIsAccessible (start.x, start.y) && fieldIsAccessible (end.x, end.y))
    {
        #if PATHFINDER_ALGO == ASTAR
            astar (start, end);
        #elif PATHFINDER_ALGO == DIJKSTRA
            dijkstra (start, end);
        #endif
    }
}

/*
 * Affichage graphique de la carte
 */
void pathfinderMapDesine ()
{
    int i, j;
    SCREEN_COLOR couleur;

    screenInit (FIELD_X / FIELD_RESOLUTION, FIELD_Y / FIELD_RESOLUTION);

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            switch (fieldMalusTerrain (i, j))
            {
                case 0 : // Blanc
                    couleur.rouge = 0xff;
                    couleur.vert = 0xff;
                    couleur.bleu = 0xff;
                    break;
                case FIELD_MALUS_TERRAIN : // Gris
                    couleur.rouge = 0xa0;
                    couleur.vert = 0xa0;
                    couleur.bleu = 0xa0;
                    break;
                case -1 :
                default : // Noir
                    couleur.rouge = 0x00;
                    couleur.vert = 0x00;
                    couleur.bleu = 0x00;
                    break;
            }

            screenSetPixel (i, j, couleur);
        }
    }

    screenRefresh ();
}

/*
 * Affichage textuel de la carte
 */
void pathfinderMapDump ()
{
    int i, j;

    printf ("{\n");
    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        printf ("{");
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            printf ("{");
            switch (fieldMalusTerrain (i, j))
            {
                case (0):
                    printf ("FIELD_MAP_ETAT_ACCESSIBLE");
                    break;
                case (FIELD_MALUS_TERRAIN):
                    printf ("FIELD_MAP_ETAT_DANGER");
                    break;
                case (-1):
                    printf ("FIELD_MAP_ETAT_INACCESSIBLE");
                    break;
            }
            printf (", PATHFINDER_NOLISTE, 0, {-1, -1, 0, 0}}");
            if (j != FIELD_Y / FIELD_RESOLUTION - 1)
            {
                printf (", ");
            }
        }
        printf ("}");
        if (i != FIELD_X / FIELD_RESOLUTION - 1)
        {
            printf (",");
        }
        printf ("\n");
    }
    printf ("}\n");
}

/*
 * Calcul de la distance entre 2 poitns en utilisant une certaine heuristique
 */
double pathfinderHeuristique (double dx, double dy)
{
    #if PATHFINDER_HEURISTIQUE == MANHATTAN
        return dx + dy;
    #elif PATHFINDER_HEURISTIQUE == EUCLIDEAN
        return sqrt (dx * dx + dy * dy);
    #elif PATHFINDER_HEURISTIQUE == OCTILE
        double F = sqrt (2) - 1;
        return (dx < dy) ? F * dx + dy : F * dy + dx;
    #elif PATHFINDER_HEURISTIQUE == CHEBYSHEV
        if (dx > dy)
        {
            return dx;
        }
        else
        {
            return dy;
        }
    #endif
}

/*
 * Calcule l'angle entre 2 points adjacents
 */
double pathfinderAngle (PATHFINDER_POINT parent, PATHFINDER_POINT point)
{
    double angle;

    if (parent.x == point.x + 1 && parent.y == point.y)
    {
        angle = -90;
    }
    else if (parent.x == point.x + 1 && parent.y == point.y + 1)
    {
        angle = -45;
    }
    else if (parent.x == point.x && parent.y == point.y + 1)
    {
        angle = 0;
    }
    else if (parent.x == point.x - 1 && parent.y == point.y + 1)
    {
        angle = 45;
    }
    else if (parent.x == point.x - 1 && parent.y == point.y)
    {
        angle = 90;
    }
    else if (parent.x == point.x - 1 && parent.y == point.y - 1)
    {
        angle = 135;
    }
    else if (parent.x == point.x && parent.y == point.y - 1)
    {
        angle = 180;
    }
    else if (parent.x == point.x + 1 && parent.y == point.y - 1)
    {
        angle = -135;
    }
    else
    {
        angle = atan2 ((point.x - parent.x), (parent.y - point.y)) * 180 / 3.14159265358979323846;
    }

    return angle;
}

/*
 * Calcule le delta entre 2 angles
 */
double pathfinderDeltaAngle (double angleCourant, double angleDestination)
{
    if (angleCourant == -1)
        return 0;
    else
        return angleCourant != angleDestination;
/*
    double delta;
    double sensHoraire, sensTrigo;

    sensHoraire = abs (angleDestination - angleCourant);
    if (angleDestination > angleCourant)
    {
        sensTrigo = abs (angleCourant + 360 - angleDestination);
    }
    else
    {
        sensTrigo = abs (360 - angleCourant + angleDestination);
    }

    if (sensHoraire > sensTrigo)
    {
        delta = sensTrigo;
    }
    else
    {
        delta = sensHoraire;
    }

    return delta;
*/
}