#ifndef MECA_H
#define MECA_H

typedef enum
{
  MECA_CLAMP_OPEN,
  MECA_CLAMP_CLOSE,
} mecaClamp_t;

void mecaInit(void);
void mecaSetHorizontalClamp(mecaClamp_t status);

#endif// MECA_H
