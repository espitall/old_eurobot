#include <FreeRTOS/module.h> 

void led_heartbeat(void * p)
{
  while(1) 
  {
    vTaskDelay(10000);
  }
}

int main(void)
{

  //initialize modules
  xTaskCreate(led_heartbeat, "heartbeat", 200, 0, 1, 0);

  //start scheduler
  vTaskStartScheduler();

  return 0;
}
