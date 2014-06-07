#include <util/crc16.h>
#include <stdio.h>
#include <string.h>
#include "com.h"
#include "com_config.h"
#include "config.h"

#define COM_MAGIC_START 'P'

#define COM_PAYLOAD_SYSTEM_RESET  6


static Peripheral_Descriptor_t dev[COM_ITH_NUMBER];

static SemaphoreHandle_t packet_write_mutex;
static com_packet_header_t packet_write_header;
static uint8_t packet_write_payload[256];

static com_packet_header_t packet_read_header;
static uint8_t packet_read_payload[256];


#define COM_READ_SHIFT_HEADER() do\
                              {\
                                unsigned int i;\
                                ptr = (uint8_t *)&packet_read_header;\
                                for(i = 0; i < sizeof(com_packet_header_t) - 1; i += 1, ptr += 1)\
                                {\
                                  *ptr = *(ptr + 1);\
                                }\
                                offset -= 1;\
                              }while(0)
                                
static void com_parse_received_packet(void)
{
  if((packet_read_header.dst == COM_BROADCAST) || (packet_read_header.dst == COM_ID))
  {
    switch(packet_read_header.payload_id)
    {
      case COM_PAYLOAD_SYSTEM:
        //parse system payload
        switch(packet_read_payload[0] & 0x7f)
        {
          case COM_PAYLOAD_SYSTEM_RESET:
            {
              void (*reset)(void) = 0x00;
              reset();
            }
            break;

          default:
            com_print(COM_ERROR, "System payload id 0x%02X not implemented", packet_read_payload[0] & 0x7f);
            break;
        }
        break;        
    }
  }
}

void com_read(void * p)
{
  (void)p;

  unsigned int offset = 0;  
  while(1) 
  {    
    //receive packet
    uint8_t * ptr = (uint8_t *)&packet_read_header;
    for(; offset < sizeof(com_packet_header_t); offset += 1, ptr += 1)
    {
      while(FreeRTOS_read(dev[0], ptr, 1 ) != 1);
    }
    
    if(packet_read_header.magic_start == COM_MAGIC_START)
    {
      //check header crc
      uint16_t crc = 0xffff;
      crc = _crc_ccitt_update(crc,packet_read_header.magic_start);
      crc = _crc_ccitt_update(crc,packet_read_header.payload_size);
      crc = _crc_ccitt_update(crc,packet_read_header.src);
      crc = _crc_ccitt_update(crc,packet_read_header.dst);
      crc = _crc_ccitt_update(crc,packet_read_header.payload_id);
      if(crc == packet_read_header.header_crc)
      {
        uint16_t crc = 0xffff;
        ptr = (uint8_t *)packet_read_payload;
        for(offset = 0; offset < packet_read_header.payload_size; offset += 1, ptr += 1)
        {
          while(FreeRTOS_read(dev[0], ptr, 1 ) != 1);
          crc = _crc_ccitt_update(crc, *ptr);
        }

        if(crc == packet_read_header.payload_crc)
        {
          offset = 0;
          com_parse_received_packet();
        }
        else
        {
          com_print(COM_WARNING,"bad packet received (wrong payload crc)");
          com_print(COM_WARNING," recv 0x%04X, expected 0x%04X",packet_read_header.payload_crc, crc);
          COM_READ_SHIFT_HEADER();
        }
      }
      else
      {
        com_print(COM_WARNING,"bad packet received (wrong header crc)");
        com_print(COM_WARNING," recv 0x%04X, expected 0x%04X",packet_read_header.header_crc, crc);
        COM_READ_SHIFT_HEADER();
      }
    }
    else
    {
      com_print(COM_WARNING,"bad packet received (wrong magic start)");
      COM_READ_SHIFT_HEADER();
    }
  }
}

void com_write(void *p )
{
  (void)p;

  while(1)
  {
    com_write_hook();
    vTaskDelay(COM_WRITE_COOL_TIME);
  }
}

uint8_t * com_request_write_buffer(uint8_t dest, uint8_t payload_id)
{
  //Avoid data corruption when two task try to send a packet
  while(xSemaphoreTake(packet_write_mutex, portMAX_DELAY) != pdTRUE);
  
  //fill header data of the packet
  packet_write_header.magic_start = COM_MAGIC_START;
  packet_write_header.payload_size = 0;
  packet_write_header.src = COM_ID;
  packet_write_header.dst = dest;
  packet_write_header.payload_id = payload_id;
  packet_write_header.header_crc = 0xffff;
  packet_write_header.payload_crc = 0xffff;

  return packet_write_payload;
}

void com_release_write_buffer(uint8_t size)
{
  //add last header data
  packet_write_header.payload_size = size;

  //compute crc
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.magic_start);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.payload_size);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.src);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.dst);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.payload_id);
  
  int i;
  for(i = 0; i < packet_write_header.payload_size; i += 1)
  {
    packet_write_header.payload_crc = _crc_ccitt_update(packet_write_header.payload_crc, packet_write_payload[i]);
  }
  
  //get external interfaces id
  uint32_t ith_ids = com_get_ith_hook(&packet_write_header);

  //send data
  for(i = 0; i < COM_ITH_NUMBER; i += 1)
  {
    if(ith_ids & (1 << i))
    {
      //send header
      int len = sizeof(com_packet_header_t);
      uint8_t * ptr = (uint8_t *)&packet_write_header;
      while(len > 0)
      {
        if(FreeRTOS_write(dev[i], ptr, 1) == 1)
        {
          ptr += 1;
          len -= 1;
        }
      }

      //send data
      len = packet_write_header.payload_size;
      ptr = packet_write_payload;
      while(len > 0)
      {
        if(FreeRTOS_write(dev[i], ptr, 1) == 1)
        {
          ptr += 1;
          len -= 1;
        }
      }
    }
  }

  //unlock "write" lock for other tasks
  xSemaphoreGive(packet_write_mutex);
}

void com_print(com_print_lvl_t lvl, char * fmt, ...)
{
  uint8_t * buf = com_request_write_buffer(COM_BROADCAST, COM_PAYLOAD_LOG);

  //fill payload data
  buf[0] = lvl;
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf((char *)(buf + 1), COM_MAX_PAYLOAD_LENGTH - 1, fmt, args); 
  va_end(args);
  
  com_release_write_buffer(len + 1);
}

void com_init(BaseType_t com_recv_priority, BaseType_t com_send_priority)
{
  int i;
  const char * ith_name[] = COM_ITH_NAME;
  packet_write_mutex = xSemaphoreCreateMutex();
  for(i = 0; i < COM_ITH_NUMBER; i += 1)
  {
    dev[i] = FreeRTOS_open((int8_t *)ith_name[i], 0);
    if(dev[i] != NULL)
    {
      FreeRTOS_ioctl(dev[i], ioctlUSE_CHARACTER_QUEUE_RX, ( void * )64 );
      FreeRTOS_ioctl(dev[i], ioctlUSE_CHARACTER_QUEUE_TX, ( void * )10 );
      FreeRTOS_ioctl(dev[i], ioctlSET_SPEED, ( void * )(19200UL ) );
      FreeRTOS_ioctl(dev[i], ioctlUSE_INTERRUPTS, ( void * )pdTRUE );
    }
  }
  xTaskCreate(com_read, "com_recv", 200, 0, com_recv_priority, 0);
  xTaskCreate(com_write, "com_send", 512, 0, com_send_priority, 0);
}

