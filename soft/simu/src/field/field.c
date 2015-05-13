#include "../config.h"
#include "field.h"
#include <math.h>
#include "../position/position.h"
#include "../screen/screen.h"

/*
 * Permet de desiner un rectangle
 */
void _fieldDrawRect (double x, double y, double largeur, double profondeur, double angle, FIELD_MAP_ETAT type)
{
    int i, j;
    double ang_rad = (angle * M_PI) / 180;

    for (i = (x - largeur / 2) / FIELD_RESOLUTION; i < (x + largeur / 2) / FIELD_RESOLUTION ; i++)
    {
        for (j = (y - profondeur / 2) / FIELD_RESOLUTION; j < (y + profondeur / 2) / FIELD_RESOLUTION; j++)
        {
            int i_rot = x / FIELD_RESOLUTION + (i - x / FIELD_RESOLUTION) * cos (ang_rad) - (j - y / FIELD_RESOLUTION) * sin (ang_rad);
            int j_rot = y / FIELD_RESOLUTION - (i - x / FIELD_RESOLUTION) * sin (ang_rad) + (j - y / FIELD_RESOLUTION) * cos (ang_rad);

            if (i_rot >= 0 && j_rot >= 0 && i_rot < FIELD_X / FIELD_RESOLUTION && j_rot < FIELD_Y / FIELD_RESOLUTION && field_map [i_rot][j_rot].type == FIELD_MAP_ETAT_NEUTRE)
            {
                field_map[i_rot][j_rot].type = type;
            }
        }
    }
}

/*
 * Permet de desiner un cercle
 */
void _fieldDrawCircle (double x, double y, double r, FIELD_MAP_ETAT type)
{
    float sinus = 0.70710678118; // sin(45);
    double range = r / (2 * sinus);
    for (int i = r ; i >= range ; --i)
    {
        double j = sqrt (r*r - i*i);
        for (int k = -j ; k <= j ; k++)
        {
            int a, b;
            a = (int) (x - k) / FIELD_RESOLUTION;
            b = (int) (y + i) / FIELD_RESOLUTION;
            if (a >= 0 && b >= 0 && a < FIELD_X / FIELD_RESOLUTION && b < FIELD_Y / FIELD_RESOLUTION && field_map [a][b].type == FIELD_MAP_ETAT_NEUTRE)
            {
                field_map[a][b].type = type;
            }
            a = (int) (x - k) / FIELD_RESOLUTION;
            b = (int) (y - i) / FIELD_RESOLUTION;
            if (a >= 0 && b >= 0 && a < FIELD_X / FIELD_RESOLUTION && b < FIELD_Y / FIELD_RESOLUTION && field_map [a][b].type == FIELD_MAP_ETAT_NEUTRE)
            {
                field_map [a][b].type = type;
            }
            a = (int) (x + i) / FIELD_RESOLUTION;
            b = (int) (y + k) / FIELD_RESOLUTION;
            if (a >= 0 && b >= 0 && a < FIELD_X / FIELD_RESOLUTION && b < FIELD_Y / FIELD_RESOLUTION && field_map [a][b].type == FIELD_MAP_ETAT_NEUTRE)
            {
                field_map [a][b].type = type;
            }
            a = (int) (x - i) / FIELD_RESOLUTION;
            b = (int) (y - k) / FIELD_RESOLUTION;
            if (a >= 0 && b >= 0 && a < FIELD_X / FIELD_RESOLUTION && b < FIELD_Y / FIELD_RESOLUTION && field_map [a][b].type == FIELD_MAP_ETAT_NEUTRE)
            {
                field_map [a][b].type = type;
            }
        }
    }

    range = r * sinus;
    _fieldDrawRect (x - range + 1 + (2 * range - 1) / 2, y - range + 1 + (2 * range - 1) / 2, 2 * range - 1, 2 * range - 1, 0, type);
}

/*
 * Zones innaccessibles par le centre du robot
 */
void _fieldMapInitDangerInaccessible ()
{
    _fieldDrawRect (FIELD_X / 2, FIELD_DANGER_INACCESSIBLE / 2, FIELD_X, FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Bordure haute

    _fieldDrawRect (FIELD_X / 2, FIELD_Y - FIELD_DANGER_INACCESSIBLE / 2, FIELD_X, FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Bordure bas

    _fieldDrawRect (0 + FIELD_DANGER_INACCESSIBLE / 2, FIELD_Y / 2, FIELD_DANGER_INACCESSIBLE, FIELD_Y, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Bordure gauche

    _fieldDrawRect (FIELD_X - FIELD_DANGER_INACCESSIBLE / 2, FIELD_Y / 2, FIELD_DANGER_INACCESSIBLE, FIELD_Y, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Bordure droite

    _fieldDrawRect (FIELD_X / 2, (580 + FIELD_DANGER_INACCESSIBLE) / 2, 1066 + FIELD_DANGER_INACCESSIBLE * 2, 580 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Escalier

    _fieldDrawRect (1200 + 600 / 2, 2000 - 100 + 100 / 2 - FIELD_DANGER_INACCESSIBLE / 2, 600 + FIELD_DANGER_INACCESSIBLE * 2, 100 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Estrade

    _fieldDrawRect (300, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2, 70 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Distributeur 1
    _fieldDrawRect (600, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2, 70 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Distributeur 2
    _fieldDrawRect (2400, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2, 70 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Distributeur 3
    _fieldDrawRect (2700, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2, 70 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Distributeur 4

    _fieldDrawRect (70 - 22 + 22 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 800 + 400 / 2, 22 + FIELD_DANGER_INACCESSIBLE, 400, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Arrête du font
    _fieldDrawRect (0 + 400 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 800 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE, 22 + FIELD_DANGER_INACCESSIBLE * 2, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Arrête du haut
    _fieldDrawRect (0 + 400 / 2 + FIELD_DANGER_INACCESSIBLE / 2, 800 - 22 + 444 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE, 22 + FIELD_DANGER_INACCESSIBLE * 2, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Arrête du bas

    _fieldDrawRect (3000 - 70 + 22 / 2 - FIELD_DANGER_INACCESSIBLE / 2, 800 + 400 / 2, 22 + FIELD_DANGER_INACCESSIBLE, 400, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Arrête du font
    _fieldDrawRect (3000 - 400 + 400 / 2 - FIELD_DANGER_INACCESSIBLE / 2, 800 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE, 22 + FIELD_DANGER_INACCESSIBLE * 2, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Arrête du haut
    _fieldDrawRect (3000 - 400 + 400 / 2 - FIELD_DANGER_INACCESSIBLE / 2, 800 - 22 + 444 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE, 22 + FIELD_DANGER_INACCESSIBLE * 2, 0, FIELD_MAP_ETAT_DANGER_INACCESSIBLE); // Arrête du bas

    _fieldDrawRect (0 + 450 / 2, 800 + 400 / 2, 450, 400, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Terrain de départ gauche - rectangle
    _fieldDrawCircle (450, 1000, 200, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Terrain de départ gauche - cercle
    _fieldDrawRect (3000 - 450 + 450 / 2, 800 + 400 / 2, 450, 400, 0, FIELD_MAP_ETAT_EQUIPE_DROITE); // Terrain de départ droit - rectangle
    _fieldDrawCircle (3000 - 450, 1000, 200, FIELD_MAP_ETAT_EQUIPE_DROITE); // Terrain de départ droit - cercle
}

/*
 * Zones de sécurité pour éviter les obstacles
 */
void _fieldMapInitDanger ()
{
    _fieldDrawRect (FIELD_X / 2, FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, FIELD_X, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Bordure haute
    _fieldDrawRect (FIELD_X / 2, FIELD_Y - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2, FIELD_X, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Bordure bas
    _fieldDrawRect (0 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, FIELD_Y / 2, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, FIELD_Y, 0, FIELD_MAP_ETAT_DANGER); // Bordure gauche
    _fieldDrawRect (FIELD_X - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2, FIELD_Y / 2, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, FIELD_Y, 0, FIELD_MAP_ETAT_DANGER); // Bordure droite

    _fieldDrawRect (FIELD_X / 2 - 1066 / 2 - FIELD_DANGER_INACCESSIBLE - FIELD_DANGER / 2, (580 + FIELD_DANGER_INACCESSIBLE) / 2, FIELD_DANGER, 580 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER); // Escalier gauche
    _fieldDrawRect (FIELD_X / 2 + 1066 / 2 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER / 2, (580 + FIELD_DANGER_INACCESSIBLE) / 2, FIELD_DANGER, 580 + FIELD_DANGER_INACCESSIBLE, 0, FIELD_MAP_ETAT_DANGER); // Escalier droite
    _fieldDrawRect (FIELD_X / 2, 580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER / 2, 1066 + FIELD_DANGER_INACCESSIBLE * 2, FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Escalier bas
    _fieldDrawRect (FIELD_X / 2 - 1066 / 2 - FIELD_DANGER_INACCESSIBLE, 580 + FIELD_DANGER_INACCESSIBLE, FIELD_DANGER, FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Escalier gauche coin
    _fieldDrawRect (FIELD_X / 2 + 1066 / 2 + FIELD_DANGER_INACCESSIBLE, 580 + FIELD_DANGER_INACCESSIBLE, FIELD_DANGER, FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Escalier droite coin

    _fieldDrawRect (1200 + 600 / 2, 2000 - 100 + 100 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2, 600 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 100 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Estrade

    _fieldDrawRect (300, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Distributeur 1
    _fieldDrawRect (600, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Distributeur 2
    _fieldDrawRect (2400, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Distributeur 3
    _fieldDrawRect (2700, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER); // Distributeur 4

    _fieldDrawRect (70 - 22 + 22 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 800 + 400 / 2, 22 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 400, 0, FIELD_MAP_ETAT_DANGER); // Arrête du font
    _fieldDrawRect (0 + 400 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 800 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 0, FIELD_MAP_ETAT_DANGER); // Arrête du haut
    _fieldDrawRect (0 + 400 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2, 800 - 22 + 444 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 0, FIELD_MAP_ETAT_DANGER); // Arrête du bas

    _fieldDrawRect (3000 - 70 + 22 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2, 800 + 400 / 2, 22 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 400, 0, FIELD_MAP_ETAT_DANGER); // Arrête du font
    _fieldDrawRect (3000 - 400 + 400 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2, 800 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 0, FIELD_MAP_ETAT_DANGER); // Arrête du haut
    _fieldDrawRect (3000 - 400 + 400 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2, 800 - 22 + 444 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, 0, FIELD_MAP_ETAT_DANGER); // Arrête du bas

    _fieldDrawCircle (450, 1000, 200 + FIELD_DANGER, FIELD_MAP_ETAT_DANGER); // Terrain de départ gauche - cercle
    _fieldDrawCircle (3000 - 450, 1000, 200 + FIELD_DANGER, FIELD_MAP_ETAT_DANGER); // Terrain de départ droit - cercle
}

/*
 * Zones de sécurité pour éviter les obstacles
 */
void _fieldMapInitDanger2 ()
{
    _fieldDrawRect (FIELD_X / 2, FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, FIELD_X, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Bordure haute
    _fieldDrawRect (FIELD_X / 2, FIELD_Y - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2 - FIELD_DANGER2 / 2, FIELD_X, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Bordure bas
    _fieldDrawRect (0 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, FIELD_Y / 2, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, FIELD_Y, 0, FIELD_MAP_ETAT_DANGER2); // Bordure gauche
    _fieldDrawRect (FIELD_X - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2 - FIELD_DANGER2 / 2, FIELD_Y / 2, FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, FIELD_Y, 0, FIELD_MAP_ETAT_DANGER2); // Bordure droite

    _fieldDrawRect (FIELD_X / 2 - 1066 / 2 - FIELD_DANGER_INACCESSIBLE - FIELD_DANGER - FIELD_DANGER2 / 2, (580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER) / 2, FIELD_DANGER2, 580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER2); // Escalier gauche
    _fieldDrawRect (FIELD_X / 2 + 1066 / 2 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2 / 2, (580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER) / 2, FIELD_DANGER2, 580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER, 0, FIELD_MAP_ETAT_DANGER2); // Escalier droite
    _fieldDrawRect (FIELD_X / 2, 580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2 / 2, 1066 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2, FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Escalier bas
    _fieldDrawRect (FIELD_X / 2 - 1066 / 2 - FIELD_DANGER_INACCESSIBLE - FIELD_DANGER / 2, 580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER / 2, FIELD_DANGER + FIELD_DANGER2, FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Escalier gauche coin
    _fieldDrawRect (FIELD_X / 2 + 1066 / 2 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER / 2, 580 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER / 2, FIELD_DANGER + FIELD_DANGER2, FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Escalier droite coin

    _fieldDrawRect (1200 + 600 / 2, 2000 - 100 + 100 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2 - FIELD_DANGER2 / 2, 600 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 100 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Estrade

    _fieldDrawRect (300, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Distributeur 1
    _fieldDrawRect (600, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Distributeur 2
    _fieldDrawRect (2400, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Distributeur 3
    _fieldDrawRect (2700, 0 + 70 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 70 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 70 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 0, FIELD_MAP_ETAT_DANGER2); // Distributeur 4

    _fieldDrawRect (70 - 22 + 22 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 800 + 400 / 2, 22 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 400, 0, FIELD_MAP_ETAT_DANGER2); // Arrête du font
    _fieldDrawRect (0 + 400 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 800 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 0, FIELD_MAP_ETAT_DANGER2); // Arrête du haut
    _fieldDrawRect (0 + 400 / 2 + FIELD_DANGER_INACCESSIBLE / 2 + FIELD_DANGER / 2 + FIELD_DANGER2 / 2, 800 - 22 + 444 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 0, FIELD_MAP_ETAT_DANGER2); // Arrête du bas

    _fieldDrawRect (3000 - 70 + 22 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2 - FIELD_DANGER2 / 2, 800 + 400 / 2, 22 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 400, 0, FIELD_MAP_ETAT_DANGER2); // Arrête du font
    _fieldDrawRect (3000 - 400 + 400 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2 - FIELD_DANGER2 / 2, 800 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 0, FIELD_MAP_ETAT_DANGER2); // Arrête du haut
    _fieldDrawRect (3000 - 400 + 400 / 2 - FIELD_DANGER_INACCESSIBLE / 2 - FIELD_DANGER / 2 - FIELD_DANGER2 / 2, 800 - 22 + 444 - 22 + 22 / 2, 400 + FIELD_DANGER_INACCESSIBLE + FIELD_DANGER + FIELD_DANGER2, 22 + FIELD_DANGER_INACCESSIBLE * 2 + FIELD_DANGER * 2 + FIELD_DANGER2 * 2, 0, FIELD_MAP_ETAT_DANGER2); // Arrête du bas

    _fieldDrawCircle (450, 1000, 200 + FIELD_DANGER + FIELD_DANGER2, FIELD_MAP_ETAT_DANGER2); // Terrain de départ gauche - cercle
    _fieldDrawCircle (3000 - 450, 1000, 200 + FIELD_DANGER + FIELD_DANGER2, FIELD_MAP_ETAT_DANGER2); // Terrain de départ droit - cercle
}

/*
 * Initialisation de l'escalier
 */
void _fieldMapInitEscalier ()
{
//    _fieldDrawRect (967 + 22 + 100 / 2, 580 - (70 * 4) + (70 * 4) / 2, 100, 70 * 4, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE_ACTION); // Terrain gauche, tapis gauche
//    _fieldDrawRect (967 + 22 + 500 - 100 + 100 / 2, 580 - (70 * 4) + (70 * 4) / 2, 100, 70 * 4, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE_ACTION); // Terrain gauche, tapis droit
//    _fieldDrawRect (967 + 22 + 500 + 22 + 100 / 2, 580 - (70 * 4) + (70 * 4) / 2, 100, 70 * 4, 0, FIELD_MAP_ETAT_EQUIPE_DROITE_ACTION); // Terrain droit, tapis gauche
//    _fieldDrawRect (967 + 22 + 500 + 22 + 500 - 100 + 100 / 2, 580 - (70 * 4) + (70 * 4) / 2, 100, 70 * 4, 0, FIELD_MAP_ETAT_EQUIPE_DROITE_ACTION); // Terrain droit, tapis droit
    _fieldDrawRect (967 + 22 / 2, 0 + 580 / 2, 22, 580, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure gauche
    _fieldDrawRect (967 + 22 + 500 + 22 / 2, 0 + 580 / 2, 22, 580, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure centrale
    _fieldDrawRect (967 + 22 + 500 + 22 + 500 + 22 / 2, 0 + 580 / 2, 22, 580, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure droite
    _fieldDrawRect (967 + 22 + 500 / 2, 0 + 22 / 2, 500, 22, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure fond gauche
    _fieldDrawRect (967 + 22 + 500 + 22 + 500 / 2, 0 + 22 / 2, 500, 22, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Bordure fond droit
    _fieldDrawRect (967 + 22 + 500 / 2, 22 + (580 - 22) / 2, 500, 580 - 22, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE2); // Terrain gauche
    _fieldDrawRect (967 + 22 + 500 + 22 + 500 / 2, 22 + (580 - 22) / 2, 500, 580 - 22, 0, FIELD_MAP_ETAT_EQUIPE_DROITE2); // Terrain droit
}

/*
 * Initialisation des distributeur de popcorn
 */
void _fieldMapInitDistributeur ()
{
    _fieldDrawRect (300 - (70 / 2) + 70 / 2, 0 + 70 / 2, 70, 70, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 1
    _fieldDrawRect (600 - (70 / 2) + 70 / 2, 0 + 70 / 2, 70, 70, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 2
    _fieldDrawRect (2400 - (70 / 2) + 70 / 2, 0 + 70 / 2, 70, 70, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 3
    _fieldDrawRect (2700 - (70 / 2) + 70 / 2, 0 + 70 / 2, 70, 70, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Distributeur 4
}

/*
 * Initialisation de la zone de départ gauche
 */
void _fieldMapInitDepartGauche ()
{
//    _fieldDrawRect (0 + 400 / 2, 400 + 378 / 2, 400, 378, 0, FIELD_MAP_ETAT_EQUIPE_DROITE); // Zone adverse haut
//    _fieldDrawRect (0 + 400 / 2, 1200 + 22 + 378 / 2, 400, 378, 0, FIELD_MAP_ETAT_EQUIPE_DROITE); // Zone adverse bas
    _fieldDrawRect (0 + 70 / 2, 1000 - (70 / 2) + 70 / 2, 70, 70, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Plot
    _fieldDrawRect (70 - 22 + 22 / 2, 800 + 400 / 2, 22, 400, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du font
//    _fieldDrawRect (0 + 450 / 2, 800 + 400 / 2, 450, 400, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Terrain - rectangle
//    _fieldDrawCircle (450, 1000, 200, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Terrain - cercle
    _fieldDrawRect (0 + 400 / 2, 800 - 22 + 22 / 2, 400, 22, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du haut
    _fieldDrawRect (0 + 400 / 2, 800 - 22 + 444 - 22 + 22 / 2, 400, 22, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du bas
}

/*
 * Initialisation de la zone de départ droit
 */
void _fieldMapInitDepartDroit ()
{
//    _fieldDrawRect (3000 - 400 + 400 / 2, 400 + 378 / 2, 400, 378, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Zone adverse haut
//    _fieldDrawRect (3000 - 400 + 400 / 2, 1200 + 22 + 378 / 2, 400, 378, 0, FIELD_MAP_ETAT_EQUIPE_GAUCHE); // Zone adverse bas
    _fieldDrawRect (3000 - 70 + 70 / 2, 1000 - (70 / 2) + 70 / 2, 70, 70, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Plot
    _fieldDrawRect (3000 - 70 + 22 / 2, 800 + 400 / 2, 22, 400, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du font
//    _fieldDrawRect (3000 - 450 + 450 / 2, 800 + 400 / 2, 450, 400, 0, FIELD_MAP_ETAT_EQUIPE_DROITE); // Terrain - rectangle
//    _fieldDrawCircle (3000 - 450, 1000, 200, FIELD_MAP_ETAT_EQUIPE_DROITE); // Terrain - cercle
    _fieldDrawRect (3000 - 400 + 400 / 2, 800 - 22 + 22 / 2, 400, 22, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du haut
    _fieldDrawRect (3000 - 400 + 400 / 2, 800 - 22 + 444 - 22 + 22 / 2, 400, 22, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Arrête du bas
}

/*
 * Initialisation de l'estrade
 */
void _fieldMapInitEstrade ()
{
    _fieldDrawRect (1200 + 600 / 2, 2000 - 100 + 100 / 2, 600, 100, 0, FIELD_MAP_ETAT_INACCESSIBLE); // Estrade
//    _fieldDrawRect (1100 + 800 / 2, 2000 - 100 + 100 / 2, 800, 100, 0, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action
//    _fieldDrawRect (1100 + 100 + (800 - 100 - 100) / 2, 2000 - 200 + 200 / 2, 800 - 100 - 100, 200, 0, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action
//    _fieldDrawCircle (1100 + 100, 2000 - 200 + 100, 100, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action - cercle
//    _fieldDrawCircle (1100 + 800 - 100, 2000 - 200 + 100, 100, FIELD_MAP_ETAT_NEUTRE_ACTION); // Terrain d'action - cercle
}

/*
 * Initialisation des robots
 */
void _fieldMapInitBots ()
{
//    #if ROBOT == PRINCIPAL
//        #if EQUIPE == GAUCHE
//            _fieldDrawRect (posRobotGauchePrincipalX, posRobotGauchePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotGauchePrincipalA, FIELD_MAP_ETAT_HIMSELF); // Robot principal équipe gauche
//            _fieldDrawRect (posRobotGaucheSecondaireX, posRobotGaucheSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotGaucheSecondaireA, FIELD_MAP_ETAT_BOT); // Robot secondaire équipe gauche
//            _fieldDrawRect (posRobotDroitePrincipalX, posRobotDroitePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotDroitePrincipalA, FIELD_MAP_ETAT_BOT); // Robot principal équipe droite
//            _fieldDrawRect (posRobotDroiteSecondaireX, posRobotDroiteSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotDroiteSecondaireA, FIELD_MAP_ETAT_BOT); // Robot secondaire équipe droite
//        #else
//            _fieldDrawRect (posRobotGauchePrincipalX, posRobotGauchePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotGauchePrincipalA, FIELD_MAP_ETAT_BOT); // Robot principal équipe gauche
//            _fieldDrawRect (posRobotGaucheSecondaireX, posRobotGaucheSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotGaucheSecondaireA, FIELD_MAP_ETAT_BOT); // Robot secondaire équipe gauche
//            _fieldDrawRect (posRobotDroitePrincipalX, posRobotDroitePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotDroitePrincipalA, FIELD_MAP_ETAT_HIMSELF); // Robot principal équipe droite
//            _fieldDrawRect (posRobotDroiteSecondaireX, posRobotDroiteSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotDroiteSecondaireA, FIELD_MAP_ETAT_BOT); // Robot secondaire équipe droite
//        #endif
//    #else
//        #if EQUIPE == GAUCHE
//            _fieldDrawRect (posRobotGauchePrincipalX, posRobotGauchePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotGauchePrincipalA, FIELD_MAP_ETAT_BOT); // Robot principal équipe gauche
//            _fieldDrawRect (posRobotGaucheSecondaireX, posRobotGaucheSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotGaucheSecondaireA, FIELD_MAP_ETAT_HIMSELF); // Robot secondaire équipe gauche
//            _fieldDrawRect (posRobotDroitePrincipalX, posRobotDroitePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotDroitePrincipalA, FIELD_MAP_ETAT_BOT); // Robot principal équipe droite
//            _fieldDrawRect (posRobotDroiteSecondaireX, posRobotDroiteSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotDroiteSecondaireA, FIELD_MAP_ETAT_BOT); // Robot secondaire équipe droite
//        #else
//            _fieldDrawRect (posRobotGauchePrincipalX, posRobotGauchePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotGauchePrincipalA, FIELD_MAP_ETAT_BOT); // Robot principal équipe gauche
//            _fieldDrawRect (posRobotGaucheSecondaireX, posRobotGaucheSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotGaucheSecondaireA, FIELD_MAP_ETAT_BOT); // Robot secondaire équipe gauche
//            _fieldDrawRect (posRobotDroitePrincipalX, posRobotDroitePrincipalY, ROBOT_PRINCIPAL_LARGEUR, ROBOT_PRINCIPAL_PROFONDEUR, posRobotDroitePrincipalA, FIELD_MAP_ETAT_BOT); // Robot principal équipe droite
//            _fieldDrawRect (posRobotDroiteSecondaireX, posRobotDroiteSecondaireY, ROBOT_SECONDAIRE_LARGEUR, ROBOT_SECONDAIRE_PROFONDEUR, posRobotDroiteSecondaireA, FIELD_MAP_ETAT_HIMSELF); // Robot secondaire équipe droite
//        #endif
//    #endif
}

/*
 * Initialisation des balles
 */
void _fieldMapInitBalles ()
{
//    _fieldDrawCircle (posBalle1X, posBalle1Y, 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
//    _fieldDrawCircle (posBalle2X, posBalle2Y, 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
//    _fieldDrawCircle (posBalle3X, posBalle3Y, 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
//    _fieldDrawCircle (posBalle4X, posBalle4Y, 35 / 2, FIELD_MAP_ETAT_ELEMENT_BALLE);
}

/*
 * Initialisation des gobelets
 */
void _fieldMapInitGobelets ()
{
//    _fieldDrawCircle (posGebelet1X, posGebelet1Y, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 1
//    _fieldDrawCircle (posGebelet2X, posGebelet2Y, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 2
//    _fieldDrawCircle (posGebelet3X, posGebelet3Y, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 3
//    _fieldDrawCircle (posGebelet4X, posGebelet4Y, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 4
//    _fieldDrawCircle (posGebelet5X, posGebelet5Y, 95 / 2, FIELD_MAP_ETAT_ELEMENT_GOBELET); // Gobelet 5
}

/*
 * Initialisation des plots
 */
void _fieldMapInitPlots ()
{
//    _fieldDrawCircle (posPlotGauche1X, posPlotGauche1Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 1
//    _fieldDrawCircle (posPlotGauche2X, posPlotGauche2Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 2
//    _fieldDrawCircle (posPlotGauche3X, posPlotGauche3Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 3
//    _fieldDrawCircle (posPlotGauche4X, posPlotGauche4Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 4
//    _fieldDrawCircle (posPlotGauche5X, posPlotGauche5Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 5
//    _fieldDrawCircle (posPlotGauche6X, posPlotGauche6Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 6
//    _fieldDrawCircle (posPlotGauche7X, posPlotGauche7Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 7
//    _fieldDrawCircle (posPlotGauche8X, posPlotGauche8Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE); // Spot équipe gauche 8
//    _fieldDrawCircle (posPlotDroite1X, posPlotDroite1Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 1
//    _fieldDrawCircle (posPlotDroite2X, posPlotDroite2Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 2
//    _fieldDrawCircle (posPlotDroite3X, posPlotDroite3Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 3
//    _fieldDrawCircle (posPlotDroite4X, posPlotDroite4Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 4
//    _fieldDrawCircle (posPlotDroite5X, posPlotDroite5Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 5
//    _fieldDrawCircle (posPlotDroite6X, posPlotDroite6Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 6
//    _fieldDrawCircle (posPlotDroite7X, posPlotDroite7Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 7
//    _fieldDrawCircle (posPlotDroite8X, posPlotDroite8Y, 60 / 2, FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE); // Spot équipe droite 8
}

void fieldSetPixel (int x, int y, SCREEN_COLOR color)
{
    int k, l;

    for (k = 0; k < FIELD_RESOLUTION / FIELD_RESOLUTION_AFFICHAGE; k++)
    {
        for (l = 0; l < FIELD_RESOLUTION / FIELD_RESOLUTION_AFFICHAGE; l++)
        {
            screenSetPixel((x * FIELD_RESOLUTION / FIELD_RESOLUTION_AFFICHAGE) + k, (y * FIELD_RESOLUTION / FIELD_RESOLUTION_AFFICHAGE) + l, color);
        }
    }
}

/*
 * Initialisation de la carte
 */
void fieldMapInit ()
{
    int i, j = 0;
    for (i = 0; i < FIELD_X / FIELD_RESOLUTION; i++)
    {
        for (j = 0; j < FIELD_Y / FIELD_RESOLUTION; j++)
        {
            FIELD_MAP_POINT point;
            point.type = FIELD_MAP_ETAT_NEUTRE;
            field_map [i][j] = point;
        }
    }

    _fieldMapInitBots ();

    _fieldMapInitBalles ();
    _fieldMapInitGobelets ();
    _fieldMapInitPlots ();

    _fieldMapInitEscalier ();
    _fieldMapInitDistributeur ();
    _fieldMapInitDepartGauche ();
    _fieldMapInitDepartDroit ();
    _fieldMapInitEstrade ();

    _fieldMapInitDangerInaccessible ();
    _fieldMapInitDanger ();
    _fieldMapInitDanger2 ();
}

void fieldMapInit2 ()
{
    FIELD_MAP_POINT point;
    #if ROBOT == PRINCIPAL
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [8][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [8][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [9][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [9][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [10][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [10][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [10][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [10][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [10][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [11][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [11][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [11][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [11][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [12][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [12][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [12][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [12][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [13][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [13][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [13][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [13][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [14][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [14][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [14][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [14][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [15][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [15][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [15][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [15][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [16][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [16][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [16][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [16][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [17][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [17][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [17][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [17][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [18][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [18][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [18][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [18][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [19][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [19][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [19][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [19][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [19][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [20][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [20][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [21][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [21][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][19] = point;
    #else
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [0][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [1][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [1][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [1][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [1][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [1][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [2][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [2][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [2][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [2][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [3][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [3][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [3][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [3][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [4][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [4][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [4][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [4][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [4][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [5][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [5][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [6][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [6][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [6][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [7][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [7][15] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [7][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [7][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [7][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [7][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [8][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [8][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [8][15] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [8][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [8][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [8][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [8][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [9][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][15] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [9][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [9][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [9][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [9][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [10][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [10][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [10][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [10][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [10][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [10][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [10][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [11][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [11][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [11][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [11][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [11][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [11][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [12][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [12][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [12][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [12][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [12][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [12][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [13][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [13][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [13][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [13][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [13][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [13][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [14][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [14][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [14][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [14][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [14][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [14][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [15][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [15][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [15][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [15][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [15][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [15][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [16][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [16][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [16][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [16][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [16][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [16][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [17][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [17][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [17][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [17][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [17][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [17][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [18][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [18][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [18][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [18][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [18][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [18][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [19][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [19][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [19][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [19][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [19][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [19][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [19][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [20][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][8] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][9] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][10] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][11] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][15] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [20][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [20][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [20][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [20][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][12] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [21][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [21][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [21][15] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [21][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [21][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [21][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [21][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][7] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][8] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][9] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][10] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][13] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [22][14] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [22][15] = point;
    point.type = FIELD_MAP_ETAT_NEUTRE;
    field_map [22][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [22][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [22][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [22][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][6] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][11] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER2;
    field_map [23][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [23][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [23][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][2] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][3] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][4] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][5] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][12] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][13] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][14] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][15] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [24][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [24][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [25][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [25][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [25][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [25][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [25][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [26][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [26][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [26][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [26][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [27][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [27][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [27][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [27][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][0] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [28][1] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [28][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][16] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [28][17] = point;
    point.type = FIELD_MAP_ETAT_DANGER;
    field_map [28][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [28][19] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][0] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][1] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][2] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][3] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][4] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][5] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][6] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][7] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][8] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][9] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][10] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][11] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][12] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][13] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][14] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][15] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][16] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][17] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][18] = point;
    point.type = FIELD_MAP_ETAT_INACCESSIBLE;
    field_map [29][19] = point;
    #endif
}

/*
 * Affichage graphique de la carte
 */
void fieldMapDesine ()
{
    int i, j;
    SCREEN_COLOR couleur;

    screenInit (FIELD_X / FIELD_RESOLUTION_AFFICHAGE, FIELD_Y / FIELD_RESOLUTION_AFFICHAGE);

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
                case FIELD_MAP_ETAT_BOT : // Violet
                    couleur.rouge = 0xff;
                    couleur.vert = 0x9d;
                    couleur.bleu = 0x00;
                    break;
                case FIELD_MAP_ETAT_HIMSELF : // Orange
                    couleur.rouge = 0xe3;
                    couleur.vert = 0x29;
                    couleur.bleu = 0xe5;
                    break;
                case FIELD_MAP_ETAT_DANGER : // Gris
                    couleur.rouge = 0x85;
                    couleur.vert = 0x85;
                    couleur.bleu = 0x85;
                    break;
                case FIELD_MAP_ETAT_DANGER2 : // Gris clair
                    couleur.rouge = 0xa0;
                    couleur.vert = 0xa0;
                    couleur.bleu = 0xa0;
                    break;
                case FIELD_MAP_ETAT_DANGER_INACCESSIBLE : // Gris foncé
                case FIELD_MAP_ETAT_EQUIPE_DROITE :
                case FIELD_MAP_ETAT_EQUIPE_GAUCHE :
                    couleur.rouge = 0x70;
                    couleur.vert = 0x70;
                    couleur.bleu = 0x70;
                    break;
                case FIELD_MAP_ETAT_NEUTRE :
                default : // Bleu
                    couleur.rouge = 0x28;
                    couleur.vert = 0x87;
                    couleur.bleu = 0xd7;
                    break;
            }

            fieldSetPixel (i, j, couleur);
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
//        case FIELD_MAP_ETAT_EQUIPE_GAUCHE :
//        case FIELD_MAP_ETAT_EQUIPE_DROITE :
        case FIELD_MAP_ETAT_ELEMENT_BALLE :
        case FIELD_MAP_ETAT_ELEMENT_GOBELET :
        case FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE :
        case FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE :
        case FIELD_MAP_ETAT_HIMSELF :
        case FIELD_MAP_ETAT_DANGER :
        case FIELD_MAP_ETAT_DANGER2 :
            return 1;
        default :
            return 0;
    }
}

/*
 * Indique le malus de terrain
 */
int fieldMalusTerrain (int x, int y)
{
    switch (field_map[x][y].type)
    {
        case FIELD_MAP_ETAT_NEUTRE :
        case FIELD_MAP_ETAT_NEUTRE_ACTION :
//        case FIELD_MAP_ETAT_EQUIPE_GAUCHE :
//        case FIELD_MAP_ETAT_EQUIPE_DROITE :
        case FIELD_MAP_ETAT_ELEMENT_BALLE :
        case FIELD_MAP_ETAT_ELEMENT_GOBELET :
        case FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE :
        case FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE :
        case FIELD_MAP_ETAT_HIMSELF :
            return 0;
        case FIELD_MAP_ETAT_DANGER :
            return FIELD_MALUS_TERRAIN;
        case FIELD_MAP_ETAT_DANGER2 :
            return FIELD_MALUS_TERRAIN2;
        default :
            return -1;
    }
}
