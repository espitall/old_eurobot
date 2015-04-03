#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>
#include "field.h"
#include "../tools/tools.h"

/*
 * Permet de desiner un rectangle
 */
void fieldDrawRect(int x, int y, int largeur, int profondeur, int type)
{
    int i, j;

    for (i = x / FIELD_RESOLUTION ; i < (x + largeur) / FIELD_RESOLUTION ; i++)
    {
        for (j = y / FIELD_RESOLUTION ; j < (y + profondeur) / FIELD_RESOLUTION; j++)
        {
            fieldMatrix[i][j] = type;
        }
    }
}

/*
 * Permet de desiner un cercle
 */
void fieldDrawCircle(int x, int y, int r, int type)
{
    float sinus = 0.70710678118; //sin(45);
    int range = r / (2*sinus);
    for (int i = r ; i >= range ; --i)
    {
        int j = sqrt(r*r - i*i);
        for (int k = -j ; k <= j ; k++)
        {
            fieldMatrix[(x-k) / FIELD_RESOLUTION][(y+i) / FIELD_RESOLUTION] = type;
            fieldMatrix[(x-k) / FIELD_RESOLUTION][(y-i) / FIELD_RESOLUTION] = type;
            fieldMatrix[(x+i) / FIELD_RESOLUTION][(y+k) / FIELD_RESOLUTION] = type;
            fieldMatrix[(x-i) / FIELD_RESOLUTION][(y-k) / FIELD_RESOLUTION] = type;
        }
    }

    range = r * sinus;
    fieldDrawRect(x - range + 1, y - range + 1, (2 * range - 1), (2 * range - 1), type);
}

/*
 * Initialisation de la matrice
 */
void fieldMatrixInit()
{
    // Escalier
    fieldDrawRect(967, 0, 22, 580, FIELD_TERRAIN_INACCESSIBLE); // Bordure gauche
    fieldDrawRect(967 + 22 + 500, 0, 22, 580, FIELD_TERRAIN_INACCESSIBLE); // Bordure centrale
    fieldDrawRect(967 + 22 + 500 + 22 + 500, 0, 22, 580, FIELD_TERRAIN_INACCESSIBLE); // Bordure droite
    fieldDrawRect(967 + 22, 0, 500, 22, FIELD_TERRAIN_INACCESSIBLE); // Bordure fond gauche
    fieldDrawRect(967 + 22 + 500 + 22, 0, 500, 22, FIELD_TERRAIN_INACCESSIBLE); // Bordure fond droit
    fieldDrawRect(967 + 22, 22, 500, 580 - 22, FIELD_TERRAIN_EQUIPE_GAUCHE2); // Terrain gauche
    fieldDrawRect(967 + 22, 580 - (70 * 4), 100, 70 * 4, FIELD_TERRAIN_EQUIPE_GAUCHE_ACTION); // Terrain gauche, tapis gauche
    fieldDrawRect(967 + 22 + 500 - 100, 580 - (70 * 4), 100, 70 * 4, FIELD_TERRAIN_EQUIPE_GAUCHE_ACTION); // Terrain gauche, tapis droit
    fieldDrawRect(967 + 22 + 500 + 22, 22, 500, 580 - 22, FIELD_TERRAIN_EQUIPE_DROITE2); // Terrain droit
    fieldDrawRect(967 + 22 + 500 + 22, 580 - (70 * 4), 100, 70 * 4, FIELD_TERRAIN_EQUIPE_DROITE_ACTION); // Terrain droit, tapis gauche
    fieldDrawRect(967 + 22 + 500 + 22 + 500 - 100, 580 - (70 * 4), 100, 70 * 4, FIELD_TERRAIN_EQUIPE_DROITE_ACTION); // Terrain droit, tapis droit

    // Distributeurs de popcorn
    fieldDrawRect(300 - (70 / 2), 0, 70, 70, FIELD_TERRAIN_INACCESSIBLE); // Distributeur 1
    fieldDrawRect(600 - (70 / 2), 0, 70, 70, FIELD_TERRAIN_INACCESSIBLE); // Distributeur 2
    fieldDrawRect(2400 - (70 / 2), 0, 70, 70, FIELD_TERRAIN_INACCESSIBLE); // Distributeur 3
    fieldDrawRect(2700 - (70 / 2), 0, 70, 70, FIELD_TERRAIN_INACCESSIBLE); // Distributeur 4

    // Zone de départ - Equipe gauche
    fieldDrawRect(0, 800, 450, 400, FIELD_TERRAIN_EQUIPE_GAUCHE); // Terrain - rectangle
    fieldDrawCircle(450, 1000, 200, FIELD_TERRAIN_EQUIPE_GAUCHE); // Terrain - cercle
    fieldDrawRect(0, 800 - 22, 400, 22, FIELD_TERRAIN_INACCESSIBLE); // Arrête du haut
    fieldDrawRect(0, 800 - 22 + 444 - 22, 400, 22, FIELD_TERRAIN_INACCESSIBLE); // Arrête du bas
    fieldDrawRect(0, 1000 - (70 / 2), 70, 70, FIELD_TERRAIN_INACCESSIBLE); // Plot
    fieldDrawRect(70 - 22, 800, 22, 400, FIELD_TERRAIN_INACCESSIBLE); // Arrête du font
    fieldDrawCircle(70 / 2, 1000, 35 / 2, FIELD_ELEMENT_NEUTRE); // Balle
    fieldDrawRect(0, 400, 400, 378, FIELD_TERRAIN_EQUIPE_DROITE); // Zone adverse haut
    fieldDrawRect(0, 1200 + 22, 400, 378, FIELD_TERRAIN_EQUIPE_DROITE); // Zone adverse bas

    // Zone de départ - Equipe droite
    fieldDrawRect(3000 - 450, 800, 450, 400, FIELD_TERRAIN_EQUIPE_DROITE); // Terrain - rectangle
    fieldDrawCircle(3000 - 450, 1000, 200, FIELD_TERRAIN_EQUIPE_DROITE); // Terrain - cercle
    fieldDrawRect(3000 - 400, 800 - 22, 400, 22, FIELD_TERRAIN_INACCESSIBLE); // Arrête du haut
    fieldDrawRect(3000 - 400, 800 - 22 + 444 - 22, 400, 22, FIELD_TERRAIN_INACCESSIBLE); // Arrête du bas
    fieldDrawRect(3000 - 70, 1000 - (70 / 2), 70, 70, FIELD_TERRAIN_INACCESSIBLE); // Plot
    fieldDrawRect(3000 - 70, 800, 22, 400, FIELD_TERRAIN_INACCESSIBLE); // Arrête du font
    fieldDrawCircle(3000 - (70 / 2), 1000, 35 / 2, FIELD_ELEMENT_NEUTRE); // Balle
    fieldDrawRect(3000 - 400, 400, 400, 378, FIELD_TERRAIN_EQUIPE_GAUCHE); // Zone adverse haut
    fieldDrawRect(3000 - 400, 1200 + 22, 400, 378, FIELD_TERRAIN_EQUIPE_GAUCHE); // Zone adverse bas

    // Estrade
    fieldDrawRect(1100, 2000 - 100, 800, 100, FIELD_TERRAIN_NEUTRE_ACTION); // Terrain d'action
    fieldDrawRect(1100 + 100, 2000 - 200, 800 - 100 - 100, 200, FIELD_TERRAIN_NEUTRE_ACTION); // Terrain d'action
    fieldDrawCircle(1100 + 100, 2000 - 200 + 100, 100, FIELD_TERRAIN_NEUTRE_ACTION); // Terrain d'action - cercle
    fieldDrawCircle(1100 + 800 - 100, 2000 - 200 + 100, 100, FIELD_TERRAIN_NEUTRE_ACTION); // Terrain d'action - cercle
    fieldDrawRect(1200, 2000 - 100, 600, 100, FIELD_TERRAIN_INACCESSIBLE); // Estrade
    fieldDrawCircle(1250, 2000 - (100 / 2), 35 / 2, FIELD_ELEMENT_NEUTRE); // Balle gauche
    fieldDrawCircle(1750, 2000 - (100 / 2), 35 / 2, FIELD_ELEMENT_NEUTRE); // Balle droite

    // Gobelets
    fieldDrawCircle(250, 1750, 95 / 2, FIELD_ELEMENT_NEUTRE); // Gobelet 1
    fieldDrawCircle(910, 830, 95 / 2, FIELD_ELEMENT_NEUTRE); // Gobelet 2
    fieldDrawCircle(1500, 1650, 95 / 2, FIELD_ELEMENT_NEUTRE); // Gobelet 3
    fieldDrawCircle(2090, 830, 95 / 2, FIELD_ELEMENT_NEUTRE); // Gobelet 4
    fieldDrawCircle(2750, 1750, 95 / 2, FIELD_ELEMENT_NEUTRE); // Gobelet 5

    // Spots
    fieldDrawCircle(90, 200, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 1
    fieldDrawCircle(90, 1750, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 2
    fieldDrawCircle(90, 1850, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 3
    fieldDrawCircle(850, 100, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 4
    fieldDrawCircle(850, 200, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 5
    fieldDrawCircle(870, 1355, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 6
    fieldDrawCircle(1100, 1770, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 7
    fieldDrawCircle(1300, 1400, 60 / 2, FIELD_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 8
    fieldDrawCircle(3000 - 90, 200, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 1
    fieldDrawCircle(3000 - 90, 1750, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 2
    fieldDrawCircle(3000 - 90, 1850, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 3
    fieldDrawCircle(3000 - 850, 100, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 4
    fieldDrawCircle(3000 - 850, 200, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 5
    fieldDrawCircle(3000 - 870, 1355, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 6
    fieldDrawCircle(3000 - 1100, 1770, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 7
    fieldDrawCircle(3000 - 1300, 1400, 60 / 2, FIELD_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 8
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
                case FIELD_TERRAIN_NEUTRE:
                    couleur = SDL_MapRGB(ecran->format, 0x28, 0x87, 0xd7); // Bleu
                    break;
                case FIELD_TERRAIN_NEUTRE_ACTION:
                    couleur = SDL_MapRGB(ecran->format, 0xf5, 0x05, 0x04); // Rouge
                    break;
                case FIELD_TERRAIN_INACCESSIBLE :
                    couleur = SDL_MapRGB(ecran->format, 0x72, 0x02, 0x02); // Rouge grisé
                    break;
                case FIELD_TERRAIN_EQUIPE_GAUCHE :
                case FIELD_ELEMENT_EQUIPE_GAUCHE :
                    couleur = SDL_MapRGB(ecran->format, 0xff, 0xfb, 0x00); // Jaune
                    break;
                case FIELD_TERRAIN_EQUIPE_GAUCHE2 :
                    couleur = SDL_MapRGB(ecran->format, 0xa2, 0x9f, 0x00); // Jaune grisé
                    break;
                case FIELD_TERRAIN_EQUIPE_GAUCHE_ACTION :
                case FIELD_TERRAIN_EQUIPE_DROITE_ACTION :
                    couleur = SDL_MapRGB(ecran->format, 0xa9, 0xa9, 0xa9); // Gris
                    break;
                case FIELD_TERRAIN_EQUIPE_DROITE :
                case FIELD_ELEMENT_EQUIPE_DROITE :
                    couleur = SDL_MapRGB(ecran->format, 0x68, 0xc7, 0x45); // Vert
                    break;
                case FIELD_TERRAIN_EQUIPE_DROITE2 :
                    couleur = SDL_MapRGB(ecran->format, 0x3d, 0x75, 0x28); // Vert grisé
                    break;
                case FIELD_ELEMENT_NEUTRE :
                    couleur = SDL_MapRGB(ecran->format, 0xff, 0xff, 0xff); // Blanc
                    break;
            }
            putpixel(ecran, i, j, couleur);
        }
    }


    SDL_Flip(ecran);

    pause();

    SDL_Quit();
}


/*
 * Initialisation de la matrice représentant les obstacles sur le terrain
 */
void fieldMatrixWithObstaclesInit()
{
    int i, j;

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            switch (fieldMatrix[i][j]){
                case FIELD_TERRAIN_INACCESSIBLE :
                case FIELD_TERRAIN_EQUIPE_GAUCHE2 :
                case FIELD_TERRAIN_EQUIPE_GAUCHE_ACTION :
                case FIELD_TERRAIN_EQUIPE_DROITE2 :
                case FIELD_TERRAIN_EQUIPE_DROITE_ACTION :
                    fieldMatrixWithObstacles[i][j] = FIELD_INACCESSIBLE;
                    break;
                default :
                    fieldMatrixWithObstacles[i][j] = FIELD_ACCESSIBLE;
                    break;
            }
        }
    }
}

/*
 * Affichage textuel de la matrice représentant les obstacles sur le terrain
 */
void fieldMatrixWithObstaclesAffiche()
{
    int i, j;

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            printf ("%d", fieldMatrixWithObstacles[i][j]);
        }
        printf ("\n");
    }
    printf ("\n");
}

/*
 * Affichage graphique de la matrice représentant les obstacles sur le terrain
 */
void fieldMatrixWithObstaclesDesine()
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
            switch(fieldMatrixWithObstacles[i][j]){
                case FIELD_INACCESSIBLE :
                    couleur = SDL_MapRGB(ecran->format, 0x00, 0x00, 0x00); // Noir
                    break;
                default :
                    couleur = SDL_MapRGB(ecran->format, 0xff, 0xff, 0xff); // Blanc
                    break;
            }
            putpixel(ecran, i, j, couleur);
        }
    }


    SDL_Flip(ecran);

    pause();

    SDL_Quit();
}