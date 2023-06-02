#include <avr/io.h>
#include <avr/interrupt.h>
#include <led.h>
#include <display.h>
#include <timer.h>

#define MULTIPLE 250

volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;

void writeTimeAndWait(uint8_t minutes, uint8_t seconds, int delay) {
  // Write the second digit of the seconds to segment 3
  writeNumberAndWait(seconds % 10, delay);

  // Write the first digit of the seconds to segment 2
  writeNumberAndWait(seconds / 10, delay);

  // Write the second digit of the minutes to segment 1
  writeNumberAndWait(minutes % 10, delay);

  // Write the first digit of the minutes to segment 0
  writeNumberAndWait(minutes / 10, delay);
}

void tick() {
  seconds++;

  if (seconds >= 60) {
    seconds = 0;
    minutes++;
  }
}

ISR(TIMER2_COMPA_vect) {
  static uint8_t counter = 0;
  counter++;

  if (counter == MULTIPLE) {
    counter = 0;
    tick();
  }
}

int main() {
  initTimer2();
  startTimer2();

  while (1) {
    // Display the minutes and seconds on the LED display
    writeTimeAndWait(minutes, seconds, 1000);
  }

  return 0;
}
