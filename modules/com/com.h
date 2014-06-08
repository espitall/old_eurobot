#ifndef COM_H
#define COM_H

#include <FreeRTOS/module.h> 
#include "payload.h"

#define COM_MAX_PAYLOAD_LENGTH 256

typedef enum
{
  COM_SBRAIN = 0,
  COM_PBRAIN,
  COM_UI,

  COM_BROADCAST = 0xff,
} com_node_id_t;

typedef enum 
{
  COM_DEBUG,
  COM_INFO,
  COM_WARNING,
  COM_ERROR,
} com_print_lvl_t;


typedef struct 
{
  uint8_t magic_start;
  uint8_t payload_size;
  uint8_t src;
  uint8_t dst;
  uint8_t payload_type;
  uint16_t header_crc;
  uint16_t payload_crc;
} com_packet_header_t;

void com_init(BaseType_t com_recv_priority, BaseType_t com_send_priority);

uint8_t * com_request_write_buffer(uint8_t dest, uint8_t payload_type);
void com_release_write_buffer(uint8_t size);

void com_print(com_print_lvl_t lvl, char * fmt, ...);


//user function prototype
uint32_t com_get_ith_hook(com_packet_header_t * header);
void com_write_hook(uint32_t tickCount);
unsigned int com_read_hook(com_packet_header_t * header, uint8_t * buf);

#endif// COM_H
