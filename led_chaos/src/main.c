#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <led.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(0);
  enableAllLeds();
  while (1) {
    lightUpOneLed(rand() % 4);
    _delay_ms(rand() % 901 + 100);
    lightDownAllLeds();
    }
  return 0;
}