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
enum {FIELD_ACCESSIBLE, FIELD_INACCESSIBLE, FIELD_OCCUPE};

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
