#include "led.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h> 

void enableOneLed ( int led ) {
  if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return;
  LED_DDR |= ( 1 << ( LED1 + led ));
}

void enableMultipleLeds ( uint8_t leds ) {
  LED_DDR |= leds;
}

void enableAllLeds () {
  LED_DDR |= ALL_LEDS_MASK;
}

void lightUpOneLed ( int led ) {
  if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return;
  LED_PORT &= ~( 1 << ( LED1 + led ));
}

void lightUpMultipleLeds ( uint8_t leds ) {
  LED_PORT &= ~(ALL_LEDS_MASK & leds);
}

void lightUpAllLeds () {
  LED_PORT &= ~ALL_LEDS_MASK;
}

void lightDownOneLed ( int led ) {
  if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return;
  LED_PORT |= ( 1 << ( LED1 + led ));
}

void lightDownMultipleLeds ( uint8_t leds ) {
  LED_PORT |= ALL_LEDS_MASK & leds;
}

void lightDownAllLeds () {
  LED_PORT |= ALL_LEDS_MASK;
}

void lightToggleOneLed ( int led ) {
  if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return;
  LED_PORT ^= ( 1 << ( LED1 + led ));
}

void dimOneLed(int led, int percentage, int duration) {
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

void dimAllLeds(int percentage, int duration) {
  for (int i = 0; i < duration; i++) {
      for (int i = 0; i < percentage; i++) {
        lightUpAllLeds();
      }
      for (int i = 0; i < 100-percentage; i++) {
        lightDownAllLeds();
      }
    _delay_ms(1);
  }
}

void fadeInOneLed(int led, int duration) {
  for (int perc = 0; perc <= 100; perc++) {
    dimOneLed(led, perc, duration/100);
  }
}

void fadeInAllLeds(int duration) {
  for (int perc = 0; perc <= 100; perc++) {
    dimAllLeds(perc, duration/100);
  }
}

void fadeOutOneLed(int led, int duration) {
  for (int perc = 100; perc >= 0; perc--) {
    dimOneLed(led, perc, duration/100);
  }
}

void fadeOutAllLeds(int duration) {
  for (int perc = 100; perc >= 0; perc--) {
    dimAllLeds(perc, duration/100);
  }
}

int ledLitUp(int led) {
  if ( led < 0 || led > NUMBER_OF_LEDS-1 ) return 0;
  if ( bit_is_set( LED_PORT, LED1 + led ) )
  {
      return 1;
  }
  return 0;
}