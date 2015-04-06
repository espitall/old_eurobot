#include "../config.h"
#include "field.h"
#include <math.h>
#include "../position/position.h"
#include "../screen/screen.h"

/*
 * Permet de desiner un rectangle
 */
void _fieldDrawRect (int x, int y, int largeur, int profondeur, int angle, FIELD_MAP_ETAT type)
{
    int i, j;

    for (i = x / FIELD_RESOLUTION ; i < (x + largeur) / FIELD_RESOLUTION ; i++)
    {
        for (j = y / FIELD_RESOLUTION ; j < (y + profondeur) / FIELD_RESOLUTION; j++)
        {
            field_map [i][j].type = type;
        }
    }

    /* TODO : tenir compte de l'angle */
    (void) angle;
}

/*
 * Permet de desiner un cercle
 */
void _fieldDrawCircle (int x, int y, int r, FIELD_MAP_ETAT type)
{
    float sinus = 0.70710678118; // sin(45);
    int range = r / (2 * sinus);
    for (int i = r ; i >= range ; --i)
    {
        int j = sqrt (r*r - i*i);
        for (int k = -j ; k <= j ; k++)
        {
            field_map [(x - k) / FIELD_RESOLUTION][(y + i) / FIELD_RESOLUTION].type = type;
            field_map [(x - k) / FIELD_RESOLUTION][(y - i) / FIELD_RESOLUTION].type = type;
            field_map [(x + i) / FIELD_RESOLUTION][(y + k) / FIELD_RESOLUTION].type = type;
            field_map [(x - i) / FIELD_RESOLUTION][(y - k) / FIELD_RESOLUTION].type = type;
        }
    }

    range = r * sinus;
    _fieldDrawRect (x - range + 1, y - range + 1, (2 * range - 1), (2 * range - 1), 90, type);
}

/*
 * Initialisation de l'escalier
 */
void _fieldMapInitEscalier ()
{
    _fieldDrawRect (967, 0, 22, 580, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure gauche
    _fieldDrawRect (967 + 22 + 500, 0, 22, 580, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure centrale
    _fieldDrawRect (967 + 22 + 500 + 22 + 500, 0, 22, 580, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure droite
    _fieldDrawRect (967 + 22, 0, 500, 22, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure fond gauche
    _fieldDrawRect (967 + 22 + 500 + 22, 0, 500, 22, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure fond droit
    _fieldDrawRect (967 + 22, 22, 500, 580 - 22, 90, FIELD_MAP_ETAT_EQUIPE_GAUCHE2); // Terrain gauche
    _fieldDrawRect (967 + 22, 580 - (70 * 4), 100, 70 * 4, 90, FIELD_MAP_ETAT_EQUIPE_GAUCHE_ACTION); // Terrain gauche, tapis gauche
    _fieldDrawRect (967 + 22 + 500 - 100, 580 - (70 * 4), 100, 70 * 4, 90, FIELD_MAP_ETAT_EQUIPE_GAUCHE_ACTION); // Terrain gauche, tapis droit
    _fieldDrawRect (967 + 22 + 500 + 22, 22, 500, 580 - 22, 90, FIELD_MAP_ETAT_EQUIPE_DROITE2); // Terrain droit
    _fieldDrawRect (967 + 22 + 500 + 22, 580 - (70 * 4), 100, 70 * 4, 90, FIELD_MAP_ETAT_EQUIPE_DROITE_ACTION); // Terrain droit, tapis gauche
    _fieldDrawRect (967 + 22 + 500 + 22 + 500 - 100, 580 - (70 * 4), 100, 70 * 4, 90, FIELD_MAP_ETAT_EQUIPE_DROITE_ACTION); // Terrain droit, tapis droit
}

/*
 * Initialisation des distributeur de popcorn
 */
void _fieldMapInitDistributeur ()
{
    _fieldDrawRect (300 - (70 / 2), 0, 70, 70, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 1
    _fieldDrawRect (600 - (70 / 2), 0, 70, 70, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 2
    _fieldDrawRect (2400 - (70 / 2), 0, 70, 70, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 3
    _fieldDrawRect (2700 - (70 / 2), 0, 70, 70, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 4
}

/*
 * Initialisation de la zone de départ gauche
 */
void _fieldMapInitDepartGauche ()
{
    _fieldDrawRect (0, 800, 450, 400, 90, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Terrain - rectangle
    _fieldDrawCircle (450, 1000, 200, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Terrain - cercle
    _fieldDrawRect (0, 800 - 22, 400, 22, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du haut
    _fieldDrawRect (0, 800 - 22 + 444 - 22, 400, 22, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du bas
    _fieldDrawRect (0, 1000 - (70 / 2), 70, 70, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Plot
    _fieldDrawRect (70 - 22, 800, 22, 400, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du font
    _fieldDrawRect (0, 400, 400, 378, 90, FIELD_MAP_ETAT_EQUIPE_DROITE); // Zone adverse haut
    _fieldDrawRect (0, 1200 + 22, 400, 378, 90, FIELD_MAP_ETAT_EQUIPE_DROITE); // Zone adverse bas
}

/*
 * Initialisation de la zone de départ droit
 */
void _fieldMapInitDepartDroit ()
{
    _fieldDrawRect (3000 - 450, 800, 450, 400, 90, FIELD_MAP_ETAT_EQUIPE_DROITE); // Terrain - rectangle
    _fieldDrawCircle (3000 - 450, 1000, 200, FIELD_MAP_ETAT_EQUIPE_DROITE); // Terrain - cercle
    _fieldDrawRect (3000 - 400, 800 - 22, 400, 22, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du haut
    _fieldDrawRect (3000 - 400, 800 - 22 + 444 - 22, 400, 22, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du bas
    _fieldDrawRect (3000 - 70, 1000 - (70 / 2), 70, 70, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Plot
    _fieldDrawRect (3000 - 70, 800, 22, 400, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du font
    _fieldDrawRect (3000 - 400, 400, 400, 378, 90, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Zone adverse haut
    _fieldDrawRect (3000 - 400, 1200 + 22, 400, 378, 90, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Zone adverse bas
}

/*
 * Initialisation de l'estrade
 */
void _fieldMapInitEstrade ()
{
    _fieldDrawRect (1100, 2000 - 100, 800, 100, 90, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action
    _fieldDrawRect (1100 + 100, 2000 - 200, 800 - 100 - 100, 200, 90, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action
    _fieldDrawCircle (1100 + 100, 2000 - 200 + 100, 100, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action - cercle
    _fieldDrawCircle (1100 + 800 - 100, 2000 - 200 + 100, 100, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action - cercle
    _fieldDrawRect (1200, 2000 - 100, 600, 100, 90, FIELD_MAP_ETAT_INACCESSIBLE); // Estrade
}

/*
 * Initialisation des robots
 */
void _fieldMapInitBots ()
{
    _fieldDrawRect (posGetXmm () - ROBOT_X / 2, posGetYmm () - ROBOT_Y / 2, ROBOT_X, ROBOT_Y, posGetAdeg (), FIELD_MAP_ETAT_BOT); // Robot principal équipe gauche
    //_fieldDrawRect (posGetXmm (), posGetYmm (), ROBOT_X, ROBOT_Y, posGetAdeg (), FIELD_MAP_ETAT_BOT); // Robot secondaire équipe gauche
    //_fieldDrawRect (posGetXmm (), posGetYmm (), ROBOT_X, ROBOT_Y, posGetAdeg (), FIELD_MAP_ETAT_BOT); // Robot principal équipe droite
    //_fieldDrawRect(posGetXmm (), posGetYmm (), ROBOT_X, ROBOT_Y, posGetAdeg (), FIELD_MAP_ETAT_BOT); // Robot secondaire équipe droite
}

/*
 * Initialisation des balles
 */
void _fieldMapInitBalles ()
{
    _fieldDrawCircle (70 / 2, 1000, 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
    _fieldDrawCircle (3000 - (70 / 2), 1000, 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
    _fieldDrawCircle (1250, 2000 - (100 / 2), 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
    _fieldDrawCircle (1750, 2000 - (100 / 2), 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
}

/*
 * Initialisation des gobelets
 */
void _fieldMapInitGobelets ()
{
    _fieldDrawCircle (250, 1750, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 1
    _fieldDrawCircle (910, 830, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 2
    _fieldDrawCircle (1500, 1650, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 3
    _fieldDrawCircle (2090, 830, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 4
    _fieldDrawCircle (2750, 1750, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 5
}

/*
 * Initialisation des plots
 */
void _fieldMapInitPlots ()
{
    _fieldDrawCircle (90, 200, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 1
    _fieldDrawCircle (90, 1750, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 2
    _fieldDrawCircle (90, 1850, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 3
    _fieldDrawCircle (850, 100, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 4
    _fieldDrawCircle (850, 200, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 5
    _fieldDrawCircle (870, 1355, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 6
    _fieldDrawCircle (1100, 1770, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 7
    _fieldDrawCircle (1300, 1400, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 8
    _fieldDrawCircle (3000 - 90, 200, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 1
    _fieldDrawCircle (3000 - 90, 1750, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 2
    _fieldDrawCircle (3000 - 90, 1850, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 3
    _fieldDrawCircle (3000 - 850, 100, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 4
    _fieldDrawCircle (3000 - 850, 200, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 5
    _fieldDrawCircle (3000 - 870, 1355, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 6
    _fieldDrawCircle (3000 - 1100, 1770, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 7
    _fieldDrawCircle (3000 - 1300, 1400, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 8
}

/*
 * Initialisation de la carte
 */
void fieldMapInit ()
{
    _fieldMapInitEscalier ();
    _fieldMapInitDistributeur ();
    _fieldMapInitDepartGauche ();
    _fieldMapInitDepartDroit ();
    _fieldMapInitEstrade ();

    _fieldMapInitBalles ();
    _fieldMapInitGobelets ();
    _fieldMapInitPlots ();

    _fieldMapInitBots ();
}

/*
 * Affichage graphique de la carte
 */
void fieldMapDesine ()
{
    int i, j;
    SCREEN_COLOR couleur;

    screenInit (FIELD_X / FIELD_RESOLUTION, FIELD_Y / FIELD_RESOLUTION);

    for (i = 0 ; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0 ; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            switch (field_map [i][j].type)
            {
                case FIELD_MAP_ETAT_NEUTRE_ACTION : // Rouge
                    couleur.rouge = 0xf5;
                    couleur.vert = 0x05;
                    couleur.bleu = 0x04;
                    break;
                case FIELD_MAP_ETAT_INACCESSIBLE : // Rouge grisé
                    couleur.rouge = 0x72;
                    couleur.vert = 0x02;
                    couleur.bleu = 0x02;
                    break;
                case FIELD_MAP_ETAT_EQUIPE_GAUCHE :
                case FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE : // Jaune
                    couleur.rouge = 0xff;
                    couleur.vert = 0xfb;
                    couleur.bleu = 0x00;
                    break;
                case FIELD_MAP_ETAT_EQUIPE_GAUCHE2 : // Jaune grisé
                    couleur.rouge = 0xa2;
                    couleur.vert = 0x9f;
                    couleur.bleu = 0x00;
                    break;
                case FIELD_MAP_ETAT_EQUIPE_GAUCHE_ACTION :
                case FIELD_MAP_ETAT_EQUIPE_DROITE_ACTION : // Gris
                    couleur.rouge = 0xa9;
                    couleur.vert = 0xa9;
                    couleur.bleu = 0xa9;
                    break;
                case FIELD_MAP_ETAT_EQUIPE_DROITE :
                case FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE : // Vert
                    couleur.rouge = 0x68;
                    couleur.vert = 0xc7;
                    couleur.bleu = 0x45;
                    break;
                case FIELD_MAP_ETAT_EQUIPE_DROITE2 : // Vert grisé
                    couleur.rouge = 0x3d;
                    couleur.vert = 0x75;
                    couleur.bleu = 0x28;
                    break;
                case FIELD_MAP_ETAT_ELEMENT_BALLE : // Vert clair
                    couleur.rouge = 0xdd;
                    couleur.vert = 0xff;
                    couleur.bleu = 0x41;
                    break;
                case FIELD_MAP_ETAT_ELEMENT_GOBELET : // Blanc
                    couleur.rouge = 0xff;
                    couleur.vert = 0xff;
                    couleur.bleu = 0xff;
                    break;
                case FIELD_MAP_ETAT_BOT : // Orange
                    couleur.rouge = 0xff;
                    couleur.vert = 0x9d;
                    couleur.bleu = 0x00;
                    break;
                case FIELD_MAP_ETAT_NEUTRE :
                default : // Bleu
                    couleur.rouge = 0x28;
                    couleur.vert = 0x87;
                    couleur.bleu = 0xd7;
                    break;
            }

            screenSetPixel (i, j, couleur);
        }
    }

    screenRefresh ();
}

/*
 * Indique si la case de la carte est accessible
 */
int fieldIsAccessible (int x, int y)
{
    switch (field_map[x][y].type)
    {
        case FIELD_MAP_ETAT_NEUTRE :
        case FIELD_MAP_ETAT_NEUTRE_ACTION :
        case FIELD_MAP_ETAT_EQUIPE_GAUCHE :
        case FIELD_MAP_ETAT_EQUIPE_DROITE :
        case FIELD_MAP_ETAT_ELEMENT_BALLE :
        case FIELD_MAP_ETAT_ELEMENT_GOBELET :
        case FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE :
        case FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE :
        case FIELD_MAP_ETAT_BOT :
            return 1;
        default :
            return 0;
    }
}