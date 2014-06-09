#ifndef ASSERV_PAYLOAD_H
#define ASSERV_PAYLOAD_H

typedef enum {
  ASSERV_PAYLOAD_DEBUG_STREAM = 0x00,
  ASSERV_PAYLOAD_READ_D_PARAMS = 0x01,
  ASSERV_PAYLOAD_READ_A_PARAMS = 0x02,
  ASSERV_PAYLOAD_WRITE_D_PARAMS = 0x03,
  ASSERV_PAYLOAD_WRITE_A_PARAMS = 0x04,
} asserv_payload_id_t;

typedef struct {
  uint32_t timestamp;
  
  int32_t dist_set_point;
  int32_t dist_pid_set_point;
  int32_t dist_speed_set_point;
  int32_t dist_speed_feedback;
  int32_t dist_feedback;
  int32_t dist_p;
  int32_t dist_d;
  int32_t dist_output;

  int32_t angu_set_point;
  int32_t angu_pid_set_point;
  int32_t angu_speed_set_point;
  int32_t angu_speed_feedback;
  int32_t angu_feedback;
  int32_t angu_p;
  int32_t angu_d;
  int32_t angu_output;
} asserv_payload_debug_stream_t;

typedef struct {
  int32_t kp;
  int32_t kd;
  int32_t shift;
  int32_t amax;
  int32_t vmax;
} asserv_payload_params_t;

#endif// ASSERV_PAYLOAD_H
