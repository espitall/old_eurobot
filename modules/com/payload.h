#ifndef PAYLOAD_H
#define PAYLOAD_H

typedef enum 
{
  COM_PAYLOAD_SYSTEM = 0x00,
  COM_PAYLOAD_LOG = 0x01,
  COM_PAYLOAD_ASSERV = 0x02,
} com_payload_type_t;

//system
typedef enum {
  COM_PAYLOAD_SYSTEM_RESET = 0x06,
} com_payload_system_id_t;

#endif// PAYLOAD_H
