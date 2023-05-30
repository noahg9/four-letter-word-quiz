#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "usart.h"
#include "led.h"
#include "display.h"
#include "button.h"
#include "potentio.h"
#include "buzzer.h"

#define __DELAY_BACKWARD_COMPATIBLE__

char* categories[7] = {"ANML", "HMAN", "CTRY", "THNG"};
char* words[4][7] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER"},
  {"HAND", "HEAD", "FOOT", "NOSE", "BACK", "KNEE", "CHIN"},
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO"},
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE"}
};

void hideConsonants(const char *secret_word, char *visible_word) {
    for (int i = 0; i < 4; i++) {
        visible_word[i] = secret_word[i];  // Convert the character to uppercase for case-insensitive comparison
        if (!(visible_word[i] == 'A' || visible_word[i] == 'E' || visible_word[i] == 'I' || visible_word[i] == 'O' || visible_word[i] == 'U')) {
            visible_word[i] = '_';
        }
    }
    visible_word[4] = '\0';  // Add null terminator to the modified word
}

int setIndex(int button, char *visible_word) {
  int cons_count = 0;
  for (int i = 0; i < 4; i++) {
    if (visible_word[i] == '_') {
      cons_count++;
    }
    if (cons_count == button) {
      return i;
    }
  }
  return 0;
}

void nextChar(int index, char *visible_word) {
  if (visible_word[index] == '_' || visible_word[index] == 'Z') {
    visible_word[index] = 'A';
  }
  else {
    visible_word[index] = visible_word[index] + 1;
  }
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
  srand(time(NULL));

  printf("Welcome to the four-letter word quiz!");
  
  while (!buttonPushed(1)) {
    writeString("CAT?");
  }

  printf("\n1 - Next category\n2 - Select category \nCategories: animal (ANML), human (HMAN), country (CTRY), thing (THNG)");
  int cat_id = -1;

  while (!buttonPushed(2)) {
    cat_id++;
    _delay_ms(500);
    while (!buttonPushed(1) && !buttonPushed(2)) {
      writeString(categories[cat_id]);
    }
    if (cat_id == 4 && !buttonPushed(2)) {
      cat_id = 0;
    }
  }
  
  char* secret_word = words[cat_id][rand() % 7];
  char visible_word[5];
  hideConsonants(secret_word, visible_word);

  int button1_index = setIndex(1, visible_word);
  int button2_index = setIndex(2, visible_word);
  int button3_index = setIndex(3, visible_word);
  
  while (1) {
    writeString(visible_word);
    if (buttonPushed(1)) {
      _delay_ms(500);
      nextChar(button1_index, visible_word);
    }
    if (buttonPushed(2)) {
      _delay_ms(500);
      nextChar(button2_index, visible_word);
    }
    if (buttonPushed(3)) {
      _delay_ms(500);
      nextChar(button3_index, visible_word);
    }
  }

  return 0;
}
