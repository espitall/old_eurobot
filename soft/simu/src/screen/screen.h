#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>

/*
 * La fenêtre où sera affiché la carte
 */
SDL_Surface *screen;

/*
 * La structure pour les couleurs des pixels
 */
struct screen_color
{
    int rouge;
    int vert;
    int bleu;
};

typedef struct screen_color SCREEN_COLOR;

/*
 * Initialisation de la fenêtre
 */
void screenInit ();

/*
 * Changement de couleur d'un pixel de la fenêtre
 */
void screenSetPixel (int x, int y, SCREEN_COLOR couleur);

/*
 * Mise à jour de la fenêtre
 */
void screenRefresh ();

/*
 * Attente la fermeture de la fenêtre
 */
void screenPause ();

/*
 * Libération des ressources de la fenêtre
 */
void screenStop ();

#endif // SCREEN_H
