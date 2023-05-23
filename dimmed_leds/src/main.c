#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <led.h>

int main() {
  while (1) {
    fadeInLed(2, 2000);
    _delay_ms(1000);
    fadeOutLed(2, 2000);
    _delay_ms(1000);
    dimLed(0, 50, 2000);
  }
  return 0;
}