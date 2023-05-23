#include <display.h>
#include <avr/io.h>
#include <util/delay.h>

int main() {
  initDisplay();
  int counter = 0;
  while (1) {  
    int digit_1 = (counter + 1) % 10; // First digit of the display 10^3
    int digit_2 = (counter + 2) % 10; // Second digit of the display 10^2
    int digit_3 = (counter + 3) % 10; // Third digit of the display 10^1
    int digit_4 = (counter + 4) % 10; // Fourth digit of the display 10^0
    int four_digit = (digit_1 * pow(10, 3)) + (digit_2 * pow(10, 2)) + (digit_3 * pow(10, 1)) + (digit_4 * pow(10, 0));   
    writeNumberAndWait(four_digit, 1000);
    counter = (counter + 1) % 10; // Increment the counter and wrap around to 0 if it exceeds 9
  }
  return 0;
}