#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define ALL_LEDS_MASK 0b00111100
#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code

void enableOneLed (int lednumber);

void enableMultipleLeds (uint8_t leds);

void enableAllLeds ();

void lightUpOneLed (int lednumber);

void lightUpAllLeds ();

void lightDownAllLeds ();

void lightToggleOneLed (int lednumber);
