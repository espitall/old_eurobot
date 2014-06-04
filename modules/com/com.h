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

void com_init(BaseType_t comPriority);
void com_print(com_print_lvl_t lvl, char * fmt, ...);

#endif// COM_H
