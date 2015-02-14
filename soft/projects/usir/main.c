#include <stdint.h>
#include <avr/io.h>

int main(void)
{
  PORTD.DIRSET = (1 << 0);
  volatile uint32_t i;


  while(1)
  {
    for(i = 0; i < 1000; i += 1);
    PORTD.OUTTGL = (1 << 0);
  }

  return 0;
}
