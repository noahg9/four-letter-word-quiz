#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <led.h>
#include <display.h>
#include <button.h>
#include <potentio.h>
#include <buzzer.h>
#include <time.h>

char* catString[7] = {"ANML", "HMAN", "CTRY", "THNG"};
char* animalWords[7] = {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER"};
char* humanWords[7] = {"HAND", "HEAD", "FOOT", "NOSE", "BACK", "KNEE", "CHIN"};
char* countryWords[7] = {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO"};
char* thingWords[7] = {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE"};

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
  int catID = 0;
  while (!buttonPushed(2)) {
    _delay_ms(500);
    while (!buttonPushed(1) && !buttonPushed(2)) {
      writeString(catString[catID]);
    }
    catID++;
    if (catID == 4 && !buttonPushed(2)) {
      catID = 0;
    }
  }
  while (1) {
    writeString("LION");
  }
  return 0;
}
