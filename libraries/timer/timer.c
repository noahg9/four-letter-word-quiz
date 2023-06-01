#include <timer.h>

void initTimer1()
{
    TCCR1A |= _BV(WGM10); // WGM10 = 1 and WGM12 = 1 --> 8-bit Fast PWM Mode
    TCCR1B |= _BV(WGM12);

    TCCR1B |= _BV(CS12) | _BV(CS10); // CS12 = 1 and CS10 = 1 --> prescaler factor is now 1024 (= every 64 us)

    TCCR1A |= _BV(COM1B1); // Enable PWM output on OC1B pin
}