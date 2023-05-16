#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#define BUTTON1_PIN PB2
#define BUTTON2_PIN PB3
#define BUTTON3_PIN PB4
#define DISPLAY_PIN PB5
#define BUZZER_PIN PB6

#define CATEGORY_COUNT 4
#define WORD_LENGTH 4
#define MAX_ATTEMPTS 10
#define TIMEOUT_SECONDS 60

struct Puzzle {
  char category[5];
  char word[WORD_LENGTH + 1];
  int attempts;
  int time;
};

char animalWords[][WORD_LENGTH + 1] = {"duck", "bear", "lion", "frog", "fish", "bird", "goat", "deer", "mole", "crab", "orca", "seal", "tuna", "worm", "dove", "snail", "swan", "toad", "lynx", "wasp"};
char humanWords[][WORD_LENGTH + 1] = {"hand", "head", "feet", "nose", "ear", "back", "lips", "arm", "leg", "knee", "toes", "chin", "hair", "neck", "skin", "eyes", "lungs", "teeth", "nail", "heel"};
char countryWords[][WORD_LENGTH + 1] = {"iran", "peru", "cuba", "chad", "fiji", "egypt", "india", "japan", "kenya", "laos", "nepal", "qatar", "spain", "togo", "yemen", "zambia", "canada", "brazil", "sweden", "mexico"};
char thingWords[][WORD_LENGTH + 1] = {"book", "ball", "bell", "desk", "door", "fork", "lamp", "pen", "shoe", "ring", "comb", "soap", "key", "pipe", "vase", "coin", "gum", "flag", "hook", "oven"};

char* categories[CATEGORY_COUNT] = {"ANML", "HMAN", "CTRY", "THNG"};
char (*words[CATEGORY_COUNT])[WORD_LENGTH + 1] = {animalWords, humanWords, countryWords, thingWords};
int wordCounts[CATEGORY_COUNT] = {20, 20, 20, 20};

struct Puzzle* currentPuzzle = NULL;
unsigned long puzzleStartTime = 0;
volatile int gameStarted = 0;

int main() {
  return 0;
}