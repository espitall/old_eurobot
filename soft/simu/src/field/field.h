#ifndef FIELD_H
#define FIELD_H

#define FIELD_X 3000
#define FIELD_Y 2000
#define FIELD_RESOLUTION 2

/*
 * La carte représentant le terrain
 */
int field_map[FIELD_X / FIELD_RESOLUTION][FIELD_Y / FIELD_RESOLUTION];

/*
 * Les états que peuvent prendre une case de la carte
 */
enum etat_map {
    FIELD_TERRAIN_NEUTRE, // Terrain neutre
    FIELD_TERRAIN_NEUTRE_ACTION, // Terrain neutre d'action
    FIELD_TERRAIN_EQUIPE_GAUCHE, // Terrain reservée à l'équipe gauche
    FIELD_TERRAIN_EQUIPE_GAUCHE2, // Terrain 2 reservée à l'équipe gauche
    FIELD_TERRAIN_EQUIPE_GAUCHE_ACTION, // Terrain d'action reservée à l'équipe gauche
    FIELD_TERRAIN_EQUIPE_DROITE, // Terrain reservé à l'équipe droite
    FIELD_TERRAIN_EQUIPE_DROITE2, // Terrain 2 reservé à l'équipe droite
    FIELD_TERRAIN_EQUIPE_DROITE_ACTION, // Terrain d'action reservée à l'équipe droite
    FIELD_TERRAIN_INACCESSIBLE, // Terrain inacessible
    FIELD_ELEMENT_BALLE, // Les balles
    FIELD_ELEMENT_GOBELET, // Les gobelets
    FIELD_ELEMENT_EQUIPE_GAUCHE, // Element reservé à l'équipe gauche
    FIELD_ELEMENT_EQUIPE_DROITE // Element reservé à l'équipe droite
};

/*
 * Initialisation de la carte
 */
void fieldMapInit();

/*
 * Affichage textuel de la carte
 */
void fieldMapAffiche();

/*
 * Affichage graphique de la carte
 */
void fieldMapDesine();


/*
 * La matrice représentant les obstacles sur le terrain
 */
int field_matrix[FIELD_X / FIELD_RESOLUTION][FIELD_Y / FIELD_RESOLUTION];

/*
 * Les états que peuvent prendre une case de la matrice
 */
enum etat_matrix {
    FIELD_INACCESSIBLE, // Terrain inacessible
    FIELD_ACCESSIBLE // Terrain acessible
};

/*
 * Initialisation de la matrice représentant les obstacles sur le terrain
 */
void fieldMatrixInit();

/*
 * Affichage textuel de la matrice représentant les obstacles sur le terrain
 */
void fieldMatrixAffiche();

/*
 * Affichage graphique de la matrice représentant les obstacles sur le terrain
 */
void fieldMatrixDesine();
#endif // FIELD_H
