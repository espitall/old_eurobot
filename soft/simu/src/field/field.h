#ifndef FIELD_H
#define FIELD_H

#include "../pathfinder/pathfinder.h"

#define FIELD_X 3000
#define FIELD_Y 2000
#define FIELD_RESOLUTION 3

/*
 * Les états que peuvent prendre une case de la carte
 */
enum field_map_etat
{
    FIELD_MAP_ETAT_NEUTRE, // Terrain neutre
    FIELD_MAP_ETAT_NEUTRE_ACTION, // Terrain neutre d'action
    FIELD_MAP_ETAT_EQUIPE_GAUCHE, // Terrain reservée à l'équipe gauche
    FIELD_MAP_ETAT_EQUIPE_GAUCHE2, // Terrain 2 reservée à l'équipe gauche
    FIELD_MAP_ETAT_EQUIPE_GAUCHE_ACTION, // Terrain d'action reservée à l'équipe gauche
    FIELD_MAP_ETAT_EQUIPE_DROITE, // Terrain reservé à l'équipe droite
    FIELD_MAP_ETAT_EQUIPE_DROITE2, // Terrain 2 reservé à l'équipe droite
    FIELD_MAP_ETAT_EQUIPE_DROITE_ACTION, // Terrain d'action reservée à l'équipe droite
    FIELD_MAP_ETAT_INACCESSIBLE, // Terrain inacessible
    FIELD_MAP_ETAT_BOT, // Robot
    FIELD_MAP_ETAT_ELEMENT_BALLE, // Balles
    FIELD_MAP_ETAT_ELEMENT_GOBELET, // Gobelets
    FIELD_MAP_ETAT_ELEMENT_EQUIPE_GAUCHE, // Plot de l'équipe gauche
    FIELD_MAP_ETAT_ELEMENT_EQUIPE_DROITE // Plot de l'équipe droite
};

typedef enum field_map_etat FIELD_MAP_ETAT;

/*
 * La structure des cases de la carte
 */
struct field_map_point
{
    FIELD_MAP_ETAT type;
    PATHFINDER_LISTES liste;
    int f;
    int g;
    int h;
    int parent_x;
    int parent_y;
};

typedef struct field_map_point FIELD_MAP_POINT;

/*
 * La carte représentant le terrain
 */
FIELD_MAP_POINT field_map [FIELD_X / FIELD_RESOLUTION][FIELD_Y / FIELD_RESOLUTION];

/*
 * Initialisation de la carte
 */
void fieldMapInit ();

/*
 * Affichage graphique de la carte
 */
void fieldMapDesine ();

/*
 * Indique si la case de la carte est accessible
 */
int fieldIsAccessible (int x, int y);

#endif // FIELD_H
