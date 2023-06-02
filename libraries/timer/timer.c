#include <timer.h>
#include <avr/io.h>

void initTimer0() {
    // Set timer 0 to CTC mode
    TCCR0A |= _BV(WGM01);

    // Set prescaler to 64
    TCCR0B |= _BV(CS01) | _BV(CS00);

    // Set compare match register to generate interrupt every 1 millisecond
    OCR0A = 249; // Assuming a 16MHz clock and prescaler of 64 (16,000,000 / 64 / 1000 - 1)

    // Enable compare match A interrupt
    TIMSK0 |= _BV(OCIE0A);
}

void startTimer0() {
    // Start Timer 0 with prescaler 256
    TCCR0B |= (1 << CS22);
}

void stopTimer0() {
    // Stop Timer 0 by setting prescaler to 0
    TCCR0B &= ~(1 << CS22);
}

void initTimer1()
{
     // Set timer 1 to CTC mode
    TCCR1B |= _BV(WGM12);

    // Set prescaler to 1024
    TCCR1B |= _BV(CS12) | _BV(CS10);

    OCR1A = 15624; // Set OCR0A for 1-second interval

    // Enable compare match A interrupt
    TIMSK1 |= _BV(OCIE1A);
}

void startTimer1() {
    // Start Timer 1 with prescaler 256
    TCCR1B |= (1 << CS22);
}

void stopTimer1() {
    // Stop Timer 1 by setting prescaler to 0
    TCCR1B &= ~(1 << CS22);
}

void initTimer2() {
    // Configure Timer 2 in CTC mode
    TCCR2A |= (1 << WGM21);

    // Enable OCR2A interrupt
    TIMSK2 |= (1 << OCIE2A);

    // Set OCR2A value for 4ms interrupt
    OCR2A = 249;

    // Set prescaler to 256
    TCCR2B |= (1 << CS22);
}

void startTimer2() {
    // Start Timer 2 with prescaler 256
    TCCR2B |= (1 << CS22);
}

void stopTimer2() {
    // Stop Timer 2 by setting prescaler to 0
    TCCR2B &= ~(1 << CS22);
}