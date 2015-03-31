#ifndef FIELD_H
#define FIELD_H

#define FIELD_X 2000
#define FIELD_Y 3000
#define FIELD_RES 5

/*
 * Initialization of the field module
 */
void fieldInit(void);

/*
 * Print the field on the lcd
 */
void fieldPrint(void);

/*
 * La matrice représentant le terrain avec indication si les cases sont accessibles ou pas
 * 0 : non accessible
 * 1 : accessible
 */
int[][] fieldMatrix();

#endif // FIELD_H
