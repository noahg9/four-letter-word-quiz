#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>

void initADC() {
    ADMUX |= ( 1 << REFS0 );    //Set up of reference voltage. We choose 5V as reference.
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  //Determine a sample rate by setting a division factor. Used division factor: 128
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}

int main() {
  initUSART();
  initADC();
  initDisplay();
  uint16_t value = 0;
  while(1) {
    ADCSRA |= ( 1 << ADSC );    //Start the analog --> digital conversion
    loop_until_bit_is_clear( ADCSRA, ADSC );    //Wait until the conversion is completed
    uint16_t value = ADC;   //Read the result
    writeNumber(value);
  }
}