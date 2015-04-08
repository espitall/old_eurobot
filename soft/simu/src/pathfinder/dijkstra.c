#include "dijkstra.h"
#include "../field/field.h"
#include "pathfinder.h"
#include "../screen/screen.h"
#include <stdio.h>
#include <stdlib.h>

#define DIJKSTRA_DEBUG

void dijkstra (PATHFINDER_POINT start, PATHFINDER_POINT end)
{
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

    /* TODO : Mettre ici l'algo du Dijkstra */
}