#ifndef FIELD_H
#define FIELD_H

#define FIELD_X 3000
#define FIELD_Y 2000
#define FIELD_RESOLUTION 5

/*
 * La matrice représentant le terrain
 */
int fieldMatrix[FIELD_X / FIELD_RESOLUTION][FIELD_Y / FIELD_RESOLUTION];

/*
 * Les états que peuvent prendre une case de la matrice
 */
enum {
    FIELD_TERRAIN_NEUTRE, // Terrain neutre
    FIELD_TERRAIN_NEUTRE_ACTION, // Terrain neutre d'action
    FIELD_TERRAIN_EQUIPE_GAUCHE, // Terrain reservée à l'équipe gauche
    FIELD_TERRAIN_EQUIPE_GAUCHE2, // Terrain 2 reservée à l'équipe gauche
    FIELD_TERRAIN_EQUIPE_GAUCHE_ACTION, // Terrain d'action reservée à l'équipe gauche
    FIELD_TERRAIN_EQUIPE_DROITE, // Terrain reservé à l'équipe droite
    FIELD_TERRAIN_EQUIPE_DROITE2, // Terrain 2 reservé à l'équipe droite
    FIELD_TERRAIN_EQUIPE_DROITE_ACTION, // Terrain d'action reservée à l'équipe droite
    FIELD_TERRAIN_INACESSIBLE, // Terrain inacessible
    FIELD_ELEMENT_NEUTRE, // Element neutre
    FIELD_ELEMENT_EQUIPE_GAUCHE, // Element reservé à l'équipe gauche
    FIELD_ELEMENT_EQUIPE_DROITE // Element reservé à l'équipe droite
};

/*
 * Initialisation de la matrice
 */
void fieldMatrixInit();

/*
 * Affichage textuel de la matrice
 */
void fieldMatrixAffiche();

/*
 * Affichage graphique de la matrice
 */
void fieldMatrixDesine();
#endif // FIELD_H
