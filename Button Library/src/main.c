#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PINC
#define BUTTON1 PC1

void enableButton( int button ) {
  if ( button < 1 || button > 3 ) return;
  DDRC &= ~( 1 << ( PC1 + (button-1) ) );
  PORTC |= ( 1 << ( PC1 + (button-1) ) );
}

int buttonPushed( int button ) {
  if ( button < 1 || button > 3 ) return;
  if (( PINC & ( 1 << ( PC1 + (button-1) ) )) == 0 )
  {
      printf( " - Button %d pressed!\n", button );
  }
  return 0;
}

// PORTB |= ( 1 << ( PB2 + lednumber ));

int buttonReleased( int button ) {
  if ( button < 1 || button > 3 ) return;
  if ( bit_is_clear( BUTTON_PIN, BUTTON1 ) )
  {
      printf( " - Button %d released!\n", button );
  }
  return 0;
}

int main() {
  initUSART();
  enableButton(1);
  enableButton(2);
  enableButton(3);
  while (1) {
    buttonPushed(1);
    buttonPushed(2);
    buttonPushed(3);
    buttonReleased(1);
    buttonReleased(2);
    buttonReleased(3);
    _delay_ms( 200 );
  }
  return 0;
}