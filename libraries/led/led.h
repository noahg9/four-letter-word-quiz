#include <avr/io.h>

#define ALL_LEDS_MASK 0b00111100
#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED2 PB5

void enableOneLed (int led);

void enableMultipleLeds (uint8_t leds);

void enableAllLeds ();

void lightUpOneLed (int led);

void lightUpMultipleLeds (uint8_t leds);

void lightUpAllLeds ();

void lightDownOneLed (int led);

void lightDownMultipleLeds (uint8_t leds);

void lightDownAllLeds ();

void lightToggleOneLed (int led);

void dimLed(int led, int percentage, int duration);

void fadeInLed(int led, int duration);

void fadeOutLed(int led, int duration);

int ledLitUp(int led);