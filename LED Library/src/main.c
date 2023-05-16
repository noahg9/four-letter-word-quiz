#include <util/delay.h>
#include <avr/io.h>

#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code

void enableOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));
}

void enableMultipleLeds ( uint8_t leds )
{
    DDRB |= leds;
}

void enableAllLeds ()
{
    DDRB |= ( 1 << ( PB2 + 0 ));
    DDRB |= ( 1 << ( PB2 + 1 ));
    DDRB |= ( 1 << ( PB2 + 2 ));
    DDRB |= ( 1 << ( PB2 + 3 ));
}

void lightUpOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));
}

void lightUpMultipleLeds ( uint8_t leds )
{
    PORTB &= ~(leds);
}

void lightUpAllLeds ()
{
    PORTB &= ~( 1 << ( PB2 + 0 ));
    PORTB &= ~( 1 << ( PB2 + 1 ));
    PORTB &= ~( 1 << ( PB2 + 2 ));
    PORTB &= ~( 1 << ( PB2 + 3 ));
}

void lightDownOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));
}

void lightDownMultipleLeds ( uint8_t leds )
{
    PORTB |= leds;
}

void lightDownAllLeds ()
{
    PORTB |= ( 1 << ( PB2 + 0 ));
    PORTB |= ( 1 << ( PB2 + 1 ));
    PORTB |= ( 1 << ( PB2 + 2 ));
    PORTB |= ( 1 << ( PB2 + 3 ));
}

void lightToggleOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB ^= ( 1 << ( PB2 + lednumber ));
}

int main()
{
    enableAllLeds();
    _delay_ms(1000);
    lightDownAllLeds();
    _delay_ms(1000);
    lightToggleOneLed(3);
    _delay_ms(1000);
    lightUpAllLeds();
    _delay_ms(1000);
    lightDownOneLed(1);
    _delay_ms(1000);
    lightToggleOneLed(3);
    _delay_ms(1000);
    lightDownAllLeds();
    _delay_ms(1000);
    lightUpMultipleLeds(0b00001101);
}