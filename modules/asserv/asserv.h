#ifndef ASSERV_H
#define ASSERV_H

#include <stdint.h>

void asserv_init(void);
void asserv_update(void);

int32_t asserv_get_left_mot_set_point(void);
int32_t asserv_get_right_mot_set_point(void);

void asserv_set_dist_set_point(int32_t sp);
int32_t asserv_get_dist_set_point(void);
int32_t asserv_get_dist_output(void);

void asserv_set_angu_set_point(int32_t sp);
int32_t asserv_get_angu_set_point(void);
int32_t asserv_get_angu_output(void);


#endif// ASSERV_H
