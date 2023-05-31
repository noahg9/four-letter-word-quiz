#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <usart.h>
#include <led.h>
#include <display.h>
#include <button.h>
#include <potentio.h>
#include <buzzer.h>

int stage;
int cat_id;
char* secret_word;
char visible_word[5];
int button1_index;
int button2_index;
int button3_index;

ISR( PCINT1_vect )
{
  if (stage == 1) {
    if ( buttonPushed(1) )
    {
      stage++;
      lightUpAllLeds();
      _delay_ms( 500 );
      lightDownAllLeds();
    }
  }
  if (stage == 2) {
    if ( buttonPushed(1) )
    {
      cat_id++;
      lightUpAllLeds();
      _delay_ms( 500 );
      lightDownAllLeds();
      if (cat_id == 4) 
      {
        cat_id = 0;
      }
    }
    if ( buttonPushed(2) )
    {
      stage++;
      lightUpAllLeds();
      _delay_ms( 500 );
      lightDownAllLeds();
    }
  }
  if (stage == 3) {
    if (buttonPushed(1)) 
    {
      nextChar(button1_index, visible_word);
      lightUpOneLed(button1_index);
      _delay_ms( 500 );
      lightDownOneLed(button1_index);
    }
    if (buttonPushed(2) && button2_index != 4) 
    {
      nextChar(button2_index, visible_word);
      lightUpOneLed(button2_index);
      _delay_ms( 500 );
      lightDownOneLed(button2_index);
    }
    if (buttonPushed(3) && button3_index != 4) 
    {
      nextChar(button3_index, visible_word);
      lightUpOneLed(button3_index);
      _delay_ms( 500 );
      lightDownOneLed(button3_index);
    }
  }
}

struct puzzle {
  char category[5];
  char word[5];
  int attempts;
  int time;
};

char* categories[4] = {"ANML", "HMAN", "CTRY", "THNG"};
char* words[4][28] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER", "WORM", "DOVE", "WASP", "TUNA", "PUMA", "CROW", "SWAN"
  "DODO", "FLEA", "GOAT", "CRAB", "MOLE", "TOAD", "SEAL", "GNAT", "HARE", "KIWI", "MOTH", "MULE", "SLUG", "WOLF"},
  {"PAUL", "GREG", "MARY", "MARC", "KATY", "NOAH", "JAKE", "ALEX", "CODY", "JANE", "OTIS", "MAYA", "LEAH", "JOSH" 
  "LIAM", "EMMA", "KATE", "LUNA", "JACK", "OWEN", "JOHN", "LUKE", "ELLA", "LILY", "RYAN", "ADAM", "ANNA", "ELLA"},
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO", "IRAQ", "BELG", "ENGL", "IREL", "NETH", "DENM", "SWED",
  "NORW", "FINL", "GERM", "FRAN", "SPAI", "PORT", "ITAL", "MEXI", "CANA", "POLA", "BRAZ", "ARGE", "JAPA", "PHIL"},
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE", "RING", "COMB", "SOAP", "PIPE", "VASE", "COIN", "FLAG"
  "HOOK", "OVEN", "BELL", "NOTE", "WIRE", "BIKE", "SUIT", "SOCK", "TOOL", "ROAD", "FILE", "TILE", "BOMB", "SEED"}
};

void hideConsonants(const char *secret_word, char *visible_word) 
{
    for (int i = 0; i < 4; i++) 
    {
        visible_word[i] = secret_word[i];
        if (!(visible_word[i] == 'A' || visible_word[i] == 'E' || visible_word[i] == 'I' 
        || visible_word[i] == 'O' || visible_word[i] == 'U')) 
        {
            visible_word[i] = '_';
        }
    }
    visible_word[4] = '\0';  // Add null terminator
}

int setIndex(int button, char *visible_word) 
{
  int cons_count = 0;
  for (int i = 0; i < 4; i++) 
  {
    if (visible_word[i] == '_') 
    {
      cons_count++;
    }
    if (cons_count == button) 
    {
      return i;
    }
  }
  return 4;
}

void nextChar(int index, char *visible_word) 
{
  if (visible_word[index] == '_' || visible_word[index] == 'Z') 
  {
    visible_word[index] = 'A';
  }
  else {
    visible_word[index] = visible_word[index] + 1;
  }
}

void correctSound() 
{
  enableBuzzer();
  playTone(C5, 100);
}

void incorrectSound() 
{
  enableBuzzer();
  playTone(C5, 100);
}


void stage1() {
  printf("Welcome to the four-letter word quiz!");
  
  while (stage == 1) 
  {
    writeString("CAT?");
  }
}

void stage2() {
  
  printf("\n1 - Next category\n2 - Select category \nCategories: animal (ANML), human (HMAN), country (CTRY), thing (THNG)");

  while (stage == 2)
  {
    writeString(categories[cat_id]);
  }
}

void stage3() {

  secret_word = words[cat_id][rand() % 7];
  hideConsonants(secret_word, visible_word);

  button1_index = setIndex(1, visible_word);
  button2_index = setIndex(2, visible_word);
  button3_index = setIndex(3, visible_word);
  
  struct puzzle puzzle1 = {categories[cat_id], secret_word, 0, 0};

  while (1) 
  {
    writeString(visible_word);
  }
}

int main() 
{
  initUSART();
  initDisplay();
  enableAllLeds();
  lightDownAllLeds();
  enableButton(1);
  enableButton(2);
  enableButton(3);
  stage = 1;
  cat_id = 0;
  PCICR |= _BV( PCIE1 );
  PCMSK1 |= _BV( PC1 ) | _BV( PC2 ) | _BV( PC3 );
  sei();
  srand(time(NULL));
  while (1) {
    if (stage == 1) {
      stage1();
    }
    if (stage == 2) {
      stage2();
    }
    if (stage == 3) {
      stage3();
    }
  }
  return 0;
}

