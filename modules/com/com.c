#include <stdio.h>
#include "com.h"
#include "com_config.h"

Peripheral_Descriptor_t dev[COM_ITH_NUMBER];

char buf[256];

void com_task(void * p)
{
  Peripheral_Descriptor_t * ith = (Peripheral_Descriptor_t *)p;

      PORTC.DIRSET = (1 << 7);            
  while(1) 
  {
    uint8_t recv;
    if(FreeRTOS_read( *ith, &recv, 1 ) == 1)
    {
    }
  }
}

void com_print(com_print_lvl_t lvl, char * fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buf, 256, fmt, args); 
  va_end(args);

  char *ptr = buf;
  while(len > 0)
  {
    if(FreeRTOS_write(dev[0], ptr, 1) == 1)
    {
      ptr += 1;
      len -= 1;
    }
  }
}

void com_init(BaseType_t comPriority)
{
  int i;
  const char * ith_name[] = COM_ITH_NAME;
  for(i = 0; i < COM_ITH_NUMBER; i += 1)
  {
    dev[i] = FreeRTOS_open((int8_t *)ith_name[i], 0);
    if(dev[i] != NULL)
    {
      FreeRTOS_ioctl(dev[i], ioctlUSE_CHARACTER_QUEUE_RX, ( void * )64 );
      FreeRTOS_ioctl(dev[i], ioctlUSE_CHARACTER_QUEUE_TX, ( void * )10 );
      FreeRTOS_ioctl(dev[i], ioctlSET_SPEED, ( void * )(19200UL ) );
      FreeRTOS_ioctl(dev[i], ioctlUSE_INTERRUPTS, ( void * )pdTRUE );

      char taskName[configMAX_TASK_NAME_LEN];
      snprintf(taskName, configMAX_TASK_NAME_LEN, "c%s", ith_name[i]);
      xTaskCreate(com_task, taskName, 100, (void *) &dev[i], comPriority, 0);
    }
  }
}

