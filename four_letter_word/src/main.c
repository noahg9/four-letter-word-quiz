#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "usart.h"
#include "led.h"
#include "display.h"
#include "button.h"
#include "potentio.h"
#include "buzzer.h"

#define __DELAY_BACKWARD_COMPATIBLE__


char* catString[7] = {"ANML", "HMAN", "CTRY", "THNG"};
char* wordCategories[4][7] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER"},
  {"HAND", "HEAD", "FOOT", "NOSE", "BACK", "KNEE", "CHIN"},
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO"},
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE"}
};

void hideConsonants(const char *secretWord, char *visibleWord) {
    int length = strlen(secretWord);
    for (int i = 0; i < length; i++) {
        visibleWord[i] = toupper(secretWord[i]);  // Convert the character to uppercase for case-insensitive comparison
        if (!(visibleWord[i] == 'A' || visibleWord[i] == 'E' || visibleWord[i] == 'I' || visibleWord[i] == 'O' || visibleWord[i] == 'U')) {
            visibleWord[i] = '_';
        }
    }
    visibleWord[length] = '\0';  // Add null terminator to the modified word
}

void correctSound() {
  enableBuzzer();
  playTone(C5, 100);
}

void incorrectSound() {
  enableBuzzer();
  playTone(C5, 100);
}


int main() {
  initUSART();
  enableButton(1);
  enableButton(2);
  enableButton(3);
  initDisplay();
  srand(0);

  printf("Welcome to the four-letter word quiz!");
  
  while (!buttonPushed(1)) {
    writeString("CAT?");
  }

  printf("\n1 - Next category\n2 - Select category \nCategories: animal (ANML), human (HMAN), country (CTRY), thing (THNG)");
  int catID = -1;

  while (!buttonPushed(2)) {
    catID++;
    _delay_ms(500);
    while (!buttonPushed(1) && !buttonPushed(2)) {
      writeString(catString[catID]);
    }
    if (catID == 4 && !buttonPushed(2)) {
      catID = 0;
    }
  }
  
  char* secretWord = wordCategories[catID][rand() % 7];
  
  char visibleWord[5];

  hideConsonants(secretWord, visibleWord);

  while (!buttonPushed(1) && !buttonPushed(2) && !buttonPushed(3)) {
    writeString(visibleWord);
  }

  return 0;
}
