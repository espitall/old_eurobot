#include <util/crc16.h>
#include <stdio.h>
#include "com.h"
#include "com_config.h"
#include "config.h"

#define COM_MAGIC_START 'P'



static Peripheral_Descriptor_t dev[COM_ITH_NUMBER];

static SemaphoreHandle_t packet_write_mutex;
static com_packet_header_t packet_write_header;
static uint8_t packet_write_payload[256];

void com_task(void * p)
{
  (void)p;

  while(1) 
  {
    uint8_t recv;
    if(FreeRTOS_read( dev[0], &recv, 1 ) == 1)
    {
    }
  }
}

static void com_lock_write(void)
{
  while(xSemaphoreTake(packet_write_mutex, portMAX_DELAY) != pdTRUE);
}

static void com_unlock_write(void)
{
  xSemaphoreGive(packet_write_mutex);
}

void com_print(com_print_lvl_t lvl, char * fmt, ...)
{
  int i;

  //Avoid data corruption when two task try to send a packet
  com_lock_write();

  //fill header data of the packet
  packet_write_header.magic_start = COM_MAGIC_START;
  packet_write_header.payload_size = 0;
  packet_write_header.src = COM_ID;
  packet_write_header.dst = 0xff;
  packet_write_header.header_crc = 0xffff;
  packet_write_header.payload_crc = 0xffff;

  //fill payload data
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf((char *)packet_write_payload, 256, fmt, args); 
  va_end(args);
  packet_write_header.payload_size = len;

  //compute crc
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.magic_start);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.payload_size);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.src);
  packet_write_header.header_crc = _crc_ccitt_update(packet_write_header.header_crc,packet_write_header.dst);
  for(i = 0; i < packet_write_header.payload_size; i += 1)
  {
    packet_write_header.payload_crc = _crc_ccitt_update(packet_write_header.payload_crc, packet_write_payload[i]);
  }

  //get external interfaces id
  uint32_t ith_ids = com_get_ith(&packet_write_header);

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
  com_unlock_write();
}

void com_init(BaseType_t comPriority)
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
  xTaskCreate(com_task, "com", 100, 0, comPriority, 0);
}

