#include "led.h"
#include <util/delay.h>
#include <avr/io.h>

void enableOneLed ( int led ) {
    if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + led ));
}

void enableMultipleLeds ( uint8_t leds ) {
    DDRB |= leds;
}

void enableAllLeds () {
    DDRB |= ALL_LEDS_MASK;
}

void lightUpOneLed ( int lednumber ) {
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));
}

void lightUpMultipleLeds ( uint8_t leds ) {
    PORTB &= ~(ALL_LEDS_MASK & leds);
}

void lightUpAllLeds () {
    PORTB &= ~ALL_LEDS_MASK;
}

void lightDownOneLed ( int led ) {
    if ( led < 0 || led > 3 ) return;
    PORTB |= ( 1 << ( PB2 + led ));
}

void lightDownMultipleLeds ( uint8_t leds ) {
    PORTB |= ALL_LEDS_MASK & leds;
}

void lightDownAllLeds () {
    PORTB |= ALL_LEDS_MASK;
}

void lightToggleOneLed ( int led ) {
    if ( led < 0 || led > 3 ) return;
    PORTB ^= ( 1 << ( PB2 + led ));
}

void dimLed(int led, int percentage, int duration) {
  enableOneLed(led);
  for (int i = 0; i < duration; i++) {
      for (int i = 0; i < percentage; i++) {
        lightUpOneLed(led);
      }
      for (int i = 0; i < 100-percentage; i++) {
        lightDownOneLed(led);
      }
    _delay_ms(1);
  }
}

void fadeInLed(int led, int duration) {
  for (int perc = 0; perc <= 100; perc++) {
    dimLed(led, perc, duration/100);
  }
}

void fadeOutLed(int led, int duration) {
  for (int perc = 100; perc >= 0; perc--) {
    dimLed(led, perc, duration/100);
  }
}

int ledLitUp(int led) {
  if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return;
  if ( bit_is_set( LED_PORT, PB2 + led ) )
  {
      return 1;
  }
  return 0;
}