#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PINC
#define BUTTON1 PC1

void enableButton( int button );

int buttonPushed( int button );

int buttonReleased( int button );