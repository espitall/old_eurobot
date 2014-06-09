#ifndef ASSERV_H
#define ASSERV_H

#include <com/com.h>
#include <stdint.h>

void asserv_init(void);
void asserv_update(void);

int32_t asserv_get_left_mot_set_point(void);
int32_t asserv_get_right_mot_set_point(void);

void asserv_set_dist_set_point(int32_t sp);
int32_t asserv_get_dist_set_point(void);
int32_t asserv_get_dist_output(void);
int32_t asserv_get_dist_p(void);
int32_t asserv_get_dist_d(void);
int32_t asserv_get_dist_pid_set_point(void);
int32_t asserv_get_dist_speed_set_point(void);

void asserv_set_angu_set_point(int32_t sp);
int32_t asserv_get_angu_set_point(void);
int32_t asserv_get_angu_output(void);
int32_t asserv_get_angu_p(void);
int32_t asserv_get_angu_d(void);
int32_t asserv_get_angu_pid_set_point(void);
int32_t asserv_get_angu_speed_set_point(void);

void asserv_com_write_handler(uint32_t tickCount);
unsigned int asserv_com_read_handler(com_packet_header_t * header, uint8_t * buf);

#endif// ASSERV_H
