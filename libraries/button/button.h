#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PINC
#define BUTTON1 PC1

void enableButton( int button );

void buttonPushed( int button );

void buttonReleased( int button );