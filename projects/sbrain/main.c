#include <FreeRTOS/module.h> 
#include <stm32f4_stdperiph/module.h> 
#include <position/position.h> 
#include <asserv/asserv.h> 
#include "motors.h"
#include "config.h"

void led_heartbeat(void * p)
{
  (void)p;

  while(1)
  {
    //Toogle heartbeat pin at 2Hz
    GPIOG->BSRRL = (1 << 13);
    vTaskDelay(500);
    GPIOG->BSRRH = (1 << 13);
    vTaskDelay(500);
  }
}

void asserv(void *p)
{
  TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t asserv_period = 20; //ms

  while(1)
  {
    //wait until full period is ellapsed
    vTaskDelayUntil( &last_wake_time, asserv_period);

    position_update();
    asserv_update();
    motors_set_consign(asserv_get_left_mot_set_point(), asserv_get_right_mot_set_point());
  }
}

int main(void)
{
  //initialize led
  //PORTQ.DIRSET = (1 << 1);
  //PORTQ.DIRSET = (1 << 2);
  //PORTQ.DIRSET = (1 << 3);
  //PORTQ.OUTCLR = (1 << 1);
  //PORTQ.OUTCLR = (1 << 2);
  //PORTQ.OUTCLR = (1 << 3);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = (1 << 13)|(1 << 14);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);



  ////initialize modules
  //position_init();
  //asserv_init();
  //motors_init();
  xTaskCreate(led_heartbeat, "heartbeat", 200, 0, SCHED_HEARTBEAT_PRIORITY, 0);
  //xTaskCreate(asserv, "asserv", 512, 0, SCHED_ASSERV_PRIORITY, 0);


  //start scheduler
  vTaskStartScheduler();


  while(1)
  {
    GPIOG->BSRRL = (1 << 13);
    for(volatile int i = 0; i < 200000; i += 1);
    GPIOG->BSRRH = (1 << 13);
    for(volatile int i = 0; i < 200000; i += 1);
  }

  return -1;
}
