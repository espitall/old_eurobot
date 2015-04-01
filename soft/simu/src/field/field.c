#include <stdio.h>
#include <SDL/SDL.h>
#include "field.h"
#include "../tools/tools.h"

/*
 * Initialisation de la matrice
 */
void fieldMatrixInit()
{
    int i, j;

    // Escalier
    for (i = 10 ; i < 20 ; i++)
    {
        for (j = 5 ; j < 10; j++)
        {
            fieldMatrix[i][j] = FIELD_INACCESSIBLE;
        }
    }

}

/*
 * Affichage textuel de la matrice
 */
void fieldMatrixAffiche()
{
    int i, j;

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            printf ("%d", fieldMatrix[i][j]);
        }
        printf ("\n");
    }
    printf ("\n");
}

/*
 * Affichage graphique de la matrice
 */
void fieldMatrixDesine()
{
    int i, j;

    SDL_Surface *ecran = NULL;
    Uint32 couleur;
    //SDL_Surface *lignes[256] = {NULL};
    //SDL_Rect position;
    //int i = 0;

    SDL_Init(SDL_INIT_VIDEO);

    ecran = SDL_SetVideoMode(FIELD_X / FIELD_RESOLUTION, FIELD_Y / FIELD_RESOLUTION, 32, SDL_HWSURFACE);

    SDL_WM_SetCaption("Simulateur - Coupe de France de robotique 2015", NULL);

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            switch(fieldMatrix[i][j]){
                case FIELD_ACCESSIBLE:
                    couleur = SDL_MapRGB(ecran->format, 0xff, 0xff, 0x00);
                    break;
                case FIELD_INACCESSIBLE:
                    couleur = SDL_MapRGB(ecran->format, 0x00, 0xff, 0xff);
                    break;
                case FIELD_OCCUPE:
                    couleur = SDL_MapRGB(ecran->format, 0xff, 0x00, 0xff);
                    break;
            }
            putpixel(ecran, i, j, couleur);
        }
    }


    SDL_Flip(ecran);

    pause();

    SDL_Quit();
}