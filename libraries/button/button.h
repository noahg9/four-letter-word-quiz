#include <avr/io.h>

#define BUTTON_PIN PINC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

void enableButton( int button );

int buttonPushed( int button );

int buttonReleased( int button );