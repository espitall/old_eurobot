#ifndef FIELD_H
#define FIELD_H

#define FIELD_X 3000
#define FIELD_Y 2000
#define FIELD_RESOLUTION 100

/*
 * Les états que peuvent prendre une case de la carte
 */
enum field_map_etat
{
    FIELD_MAP_ETAT_INACCESSIBLE,
    FIELD_MAP_ETAT_ACCESSIBLE,
    FIELD_MAP_ETAT_DANGER,
    FIELD_MAP_ETAT_DANGER2
};

typedef enum field_map_etat FIELD_MAP_ETAT;

/*
 * Initialization of the field module
 */
void fieldInit(void);

/*
 * Print the field on the lcd
 */
void fieldPrint(void);

#endif // FIELD_H
