#include <avr/io.h>
#include <usart.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <display.h>
#include <avr/interrupt.h>
#include <button.h>
#include <potentio.h>
#include <buzzer.h>

int main()
{
  enableBuzzer();
  enableButton(1);
  while (1) {
    if (buttonPushed(1)) {
      playTone(523.250, 150);
    }
  }
  return 0;
}
