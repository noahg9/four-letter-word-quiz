#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>
#include <potentio.h>

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