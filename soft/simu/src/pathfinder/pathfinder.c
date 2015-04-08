#if PATHFINDER_ALGO == ASTAR
    #include "astar.h"
#elif PATHFINDER_ALGO == DIJKSTRA
    #include "dijkstra.h"
#endif
#include "../field/field.h"
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
    PATHFINDER_POINT start = {posGetXmm () / FIELD_RESOLUTION, posGetYmm () / FIELD_RESOLUTION, 1};
    PATHFINDER_POINT end = {x / FIELD_RESOLUTION, y / FIELD_RESOLUTION, 1};

    #if PATHFINDER_ALGO == ASTAR
        astar (start, end);
    #elif PATHFINDER_ALGO == DIJKSTRA
        dijkstra (start, end);
    #endif
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
            switch (fieldIsAccessible (i, j))
            {
                case 1 : // Blanc
                    couleur.rouge = 0xff;
                    couleur.vert = 0xff;
                    couleur.bleu = 0xff;
                    break;
                case 0 :
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
