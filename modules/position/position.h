#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

void position_init(void);
void position_update(void);

int32_t position_get_left_speed(void);
int32_t position_get_right_speed(void);

int32_t position_get_dist(void);

#endif// POSITION_H
