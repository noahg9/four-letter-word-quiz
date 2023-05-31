#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <led.h>
#include <button.h>

// Pin Definitions
#define LED_PIN_1 PB2
#define LED_PIN_2 PB3
#define LED_PIN_3 PB4
#define LED_PIN_4 PB5
#define BUTTON_PIN PD2

// Morse code durations in milliseconds
#define DOT_DURATION 200
#define DASH_DURATION 600
#define SPACE_BETWEEN_ELEMENTS 200
#define SPACE_BETWEEN_LETTERS 600
#define SPACE_BETWEEN_WORDS 1200

// Function to display countdown pattern
void displayCountdown() {
  for (int i = 4; i >= 0; i--) {
    for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
      lightUpOneLed(j);
    }
    _delay_ms(500);
    for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
      lightDownOneLed(j);
    }
    _delay_ms(500);
  }
}

// Morse code array
const char* morseCode[] = {
  ".-",    // A
  "-...",  // B
  "-.-.",  // C
  "-..",   // D
  ".",     // E
  "..-.",  // F
  "--.",   // G
  "....",  // H
  "..",    // I
  ".---",  // J
  "-.-",   // K
  ".-..",  // L
  "--",    // M
  "-.",    // N
  "---",   // O
  ".--.",  // P
  "--.-",  // Q
  ".-.",   // R
  "...",   // S
  "-",     // T
  "..-",   // U
  "...-",  // V
  ".--",   // W
  "-..-",  // X
  "-.--",  // Y
  "--.."   // Z
};

// Function to display Morse code for a character
void displayMorseCode(char character) {
  int index = toupper(character) - 'A';

  if (index >= 0 && index < 26) {
    const char* morse = morseCode[index];

    for (int i = 0; i < strlen(morse); i++) {
      if (morse[i] == '.') {
        for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
          lightUpOneLed(j);
        }
        _delay_ms(DOT_DURATION);
      } else if (morse[i] == '-') {
        for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
          lightUpOneLed(j);
        }
        _delay_ms(DASH_DURATION);
      }
      for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
        lightDownOneLed(j);
      }
      _delay_ms(SPACE_BETWEEN_ELEMENTS);
    }
  }
}

int main() {
  // Set LED pins as outputs
  enableAllLeds();

  // Set button pin as input with internal pull-up resistor
  enableAllButtons();

  // Wait for button press to start
  while (PIND & (1 << BUTTON_PIN)) {
    displayCountdown();
  }

  // Training loop
  int score = 0;
  for (int round = 1; round <= 5; round++) {
    char letter = random('A', 'Z' + 1);

    // Display Morse code for the letter
    displayMorseCode(letter);

    // Wait for user's answer
    _delay_ms(SPACE_BETWEEN_LETTERS);

    // Show correct answer
    printf("Correct answer: %c\n", letter);

    // Check user's answer
    char userAnswer = ' ';
    while (userAnswer != letter) {
      if (!(PIND & (1 << BUTTON_PIN))) {
        userAnswer = 'A' + (int)random(3); // Choose a random answer (A, B, or C)
        printf("Your answer: %c\n", userAnswer);
        break;
      }
    }

    if (userAnswer == letter) {
      score++;
    }

    if (round < 5) {
      // Space between letters
      _delay_ms(SPACE_BETWEEN_LETTERS);
    }
  }

  // Frivolous LED dance
  for (int i = 0; i < 5; i++) {
    for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
      lightUpOneLed(j);
    }
    _delay_ms(200);
    for (int j = LED_PIN_1; j <= LED_PIN_4; j++) {
      lightDownOneLed(j);
    }
    _delay_ms(200);
  }

  // Display score
  printf("Your score: %d/5\n", score);

  return 0;
}
