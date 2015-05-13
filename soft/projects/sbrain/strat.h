#ifndef STRAT_H
#define STRAT_H

typedef enum
{
  STRAT_COLOR_GREEN,
  STRAT_COLOR_YELLOW,
} strat_color_t;

void stratInit(void);
int stratGetTimeLeft(void);
void stratStart(void);
void stratWedging(void);
strat_color_t stratGetColor(void);


#endif // STRAT_H
