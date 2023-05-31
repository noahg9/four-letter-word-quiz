#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <led.h>

int main() {
  while (1) {
    fadeInOneLed(2, 2000);
    _delay_ms(1000);
    fadeOutOneLed(2, 2000);
    _delay_ms(1000);
    dimOneLed(0, 50, 2000);
  }
  return 0;
}