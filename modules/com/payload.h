#ifndef PAYLOAD_H
#define PAYLOAD_H

typedef enum 
{
  COM_PAYLOAD_SYSTEM = 0x00,
  COM_PAYLOAD_LOG = 0x01,
  COM_PAYLOAD_ASSERV = 0x02,
} com_payload_id_t;


//asserv
typedef enum {
  COM_PAYLOAD_ASSERV_DEBUG_STREAM = 0x00,
} com_payload_asserv_id_t;

typedef struct {
  uint32_t timestamp;
  
  int32_t dist_set_point;
  int32_t dist_feedback;
  int32_t dist_p;
  int32_t dist_d;
  int32_t dist_output;

  int32_t angu_set_point;
  int32_t angu_feedback;
  int32_t angu_p;
  int32_t angu_d;
  int32_t angu_output;
} com_payload_asserv_debug_stream_t;

#endif// PAYLOAD_H
