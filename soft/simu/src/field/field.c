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