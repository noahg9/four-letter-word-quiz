#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <usart.h>

int main() {
    initUSART();
    initDisplay();
  
    // Write a single character to a segment
    writeCharToSegment(0, 'E');
    _delay_ms(1000);
    writeCharToSegment(1, 'A');
    _delay_ms(1000);
    writeCharToSegment(2, 'S');
    _delay_ms(1000);
    writeCharToSegment(3, 'Y');
    _delay_ms(1000);
    // Write a string to the display
    char str[] = "HARD";
    writeString(str);
    _delay_ms(1000);
    // Write a string to the display and wait for a specified duration
    char str2[] = "EASY";
    writeStringAndWait(str2, 2000);
  
    return 0;
}