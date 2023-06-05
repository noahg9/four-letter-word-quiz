#include <button.h>
#include <stdio.h>

void enableButton( int button ) {
  if ( button < 1 || button > 3 ) return;
  DDRC &= ~( 1 << ( BUTTON1 + (button-1) ) );
  PORTC |= ( 1 << ( BUTTON1 + (button-1) ) );
}

void enableAllButtons() {
  for (int i = 0; i < 3; i++) {
    DDRC &= ~( 1 << ( BUTTON1 + (i) ) );
    PORTC |= ( 1 << ( BUTTON1 + (i) ) );
  }
}

int buttonPushed( int button ) {
  if ( button < 1 || button > 3 ) return 0;
  if (( BUTTON_PIN & ( 1 << ( BUTTON1 + (button-1) ) )) == 0 )
  {
      return 1;
  }
  return 0;
}

int buttonReleased( int button ) {
  if ( button < 1 || button > 3 ) return 0;
  if ( bit_is_clear( BUTTON_PIN, BUTTON1 ) )
  {
      return 1;
  }
  return 0;
}