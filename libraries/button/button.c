#include <button.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
      return 1;
  }
  return;
}

int buttonReleased( int button ) {
  if ( button < 1 || button > 3 ) return;
  if ( bit_is_clear( BUTTON_PIN, BUTTON1 ) )
  {
      printf( " - Button %d released!\n", button );
      return 1;
  }
  return;
}