#include <led.h>
#include <util/delay.h>
#include <avr/io.h>

void enableOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));
}

void enableMultipleLeds ( uint8_t leds )
{
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

void lightDownOneLed ( int lednumber ) {
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));
}

void lightDownMultipleLeds ( uint8_t leds ) {
    PORTB |= ALL_LEDS_MASK & leds;
}

void lightDownAllLeds () {
    PORTB |= ALL_LEDS_MASK;
}

void lightToggleOneLed ( int lednumber ) {
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB ^= ( 1 << ( PB2 + lednumber ));
}
