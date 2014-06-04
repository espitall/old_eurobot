#ifndef COM_H
#define COM_H

#include <FreeRTOS/module.h> 

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
  uint16_t header_crc;
  uint16_t payload_crc;
} com_packet_header_t;

void com_init(BaseType_t comPriority);
void com_print(com_print_lvl_t lvl, char * fmt, ...);

uint32_t com_get_ith(com_packet_header_t * header);

#endif// COM_H
