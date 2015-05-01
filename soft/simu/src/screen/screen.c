#include <SDL/SDL.h>
#include "screen.h"

/*
 * Initialisation de la fenêtre
 */
void screenInit (int largeur, int hauteur)
{
    SDL_Init (SDL_INIT_VIDEO);
    SDL_WM_SetCaption ("Simulateur - Coupe de France de robotique 2015", NULL);
    screen = SDL_SetVideoMode (largeur, hauteur, 32, SDL_HWSURFACE);
}

/*
 * Changement de couleur d'un pixel de la fenêtre
 */
void screenSetPixel (int x, int y, SCREEN_COLOR color)
{
    Uint32 couleur = SDL_MapRGB (screen->format, color.rouge, color.vert, color.bleu);

    int bpp = screen->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) screen->pixels + y * screen->pitch + x * bpp;

    switch (bpp)
    {
        case 1 :
            *p = couleur;
            break;
        case 2 :
            *(Uint16 *) p = couleur;
            break;
        case 3 :
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p [0] = (couleur >> 16) & 0xff;
                p [1] = (couleur >> 8) & 0xff;
                p [2] = couleur & 0xff;
            }
            else
            {
                p [0] = couleur & 0xff;
                p [1] = (couleur >> 8) & 0xff;
                p [2] = (couleur >> 16) & 0xff;
            }
            break;
        case 4 :
            *(Uint32 *) p = couleur;
            break;
    }
}

/*
 * Mise à jour de la fenêtre
 */
void screenRefresh ()
{
    SDL_Flip (screen);
}

/*
 * Attente la fermeture de la fenêtre
 */
void screenPause ()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent (&event);
        switch (event.type)
        {
            case SDL_QUIT :
                continuer = 0;
                break;
        }
    }
}

/*
 * Libération des ressources de la fenêtre
 */
void screenStop ()
{
    SDL_Quit ();
}