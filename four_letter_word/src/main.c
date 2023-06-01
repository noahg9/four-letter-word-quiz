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
#include <timer.h>

// Constants
#define MAX_CATEGORIES 5
#define MAX_WORDS 30
#define MAX_CATEGORY_NAME_LENGTH 5
#define MAX_WORD_LENGTH 5

typedef struct {
  char category[5];
  char word[5];
  int attempts;
  int time;
} PUZZLE;

int stage = 1;
int cat_id = 0;
char* secret_word;
char visible_word[5];
int button1_index = 0;
int button2_index = 0;
int button3_index = 0;
uint16_t adcValue = 0;

PUZZLE puzzle;

char categories[MAX_CATEGORIES][MAX_CATEGORY_NAME_LENGTH] = {"ANML", "HMAN", "CTRY", "THNG"};
char words[MAX_CATEGORIES][MAX_WORDS][MAX_WORD_LENGTH] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER", "WORM", "DOVE", "WASP", "TUNA", "PUMA", "CROW", "SWAN",
  "DODO", "FLEA", "GOAT", "CRAB", "MOLE", "TOAD", "SEAL", "GNAT", "HARE", "KIWI", "MOTH", "MULE", "SLUG", "WOLF"},
  {"PAUL", "GREG", "MARY", "MARC", "KATY", "NOAH", "JAKE", "ALEX", "CODY", "JANE", "OTIS", "MAYA", "LEAH", "JOSH",
  "LIAM", "EMMA", "KATE", "LUNA", "JACK", "OWEN", "JOHN", "LUKE", "ELLA", "LILY", "RYAN", "ADAM", "ANNA", "ELLA"},
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO", "IRAQ", "BELG", "ENGL", "IREL", "NETH", "DENM", "SWED",
  "NORW", "FINL", "GERM", "FRAN", "SPAI", "PORT", "ITAL", "MEXI", "CANA", "POLA", "BRAZ", "ARGE", "JAPA", "PHIL"},
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE", "RING", "COMB", "SOAP", "PIPE", "VASE", "COIN", "FLAG",
  "HOOK", "OVEN", "BELL", "NOTE", "WIRE", "BIKE", "SUIT", "SOCK", "TOOL", "ROAD", "FILE", "TILE", "BOMB", "SEED"}
};

ISR( PCINT1_vect )
{
  if (stage == 1) {
    if ( buttonPushed(1) )
    {
      stage++;
      _delay_ms(500);
    }
  }
  if (stage == 2) {
    if ( buttonPushed(1) )
    {
      cat_id++;
      _delay_ms(500);
      if (cat_id == 4) 
      {
        cat_id = 0;
      }
    }
    if ( buttonPushed(2) )
    {
      stage++;
      _delay_ms(500);
    }
  }
  if (stage == 3) {
    if (buttonPushed(1)) 
    {
      nextChar(button1_index, visible_word);
      lightUpAllLeds();
      lightDownOneLed(button1_index);
      _delay_ms(500);
    }
    if (buttonPushed(2) && button2_index != 4) 
    {
      nextChar(button2_index, visible_word);
      lightUpAllLeds();
      lightDownOneLed(button2_index);
      _delay_ms(500);

    }
    if (buttonPushed(3) && button3_index != 4) 
    {
      nextChar(button3_index, visible_word);
      lightUpAllLeds();
      lightDownOneLed(button3_index);
      _delay_ms(500);    
    }
  }
}

ISR(ADC_vect)
{
  adcValue = ADC;
}

void hideConsonants(const char *secret_word, char *visible_word) 
{
    for (int i = 0; i < 4; i++) 
    {
        if (secret_word[i] == 'A' || secret_word[i] == 'E' || secret_word[i] == 'I' || secret_word[i] == 'O' || secret_word[i] == 'U')
        {
            visible_word[i] = secret_word[i];
        }
        else
        {
            visible_word[i] = '_';
        }
    }
    visible_word[4] = '\0';  // Add null terminator
}

int setIndex(int button, char* visible_word) 
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
  float frequencies[] = { C5, D5, E5, F5, G5, A5, B5, C6 };
  for ( int note = 0; note < 8; note++ )
  {
    playTone( frequencies[note], 150 );
    _delay_ms( 150 ); 
  }
 }

void incorrectSound() 
{
  enableBuzzer();
  playTone(C6, 1000);
}

int main() 
{
  initUSART();
  initTimer1(); 
  initDisplay();
  initADC();
  enableAllLeds();
  lightUpAllLeds();
  enableAllButtons();
  stage = 1;
  cat_id = 0;
  PCICR |= _BV( PCIE1 );
  PCMSK1 |= _BV( PC1 ) | _BV( PC2 ) | _BV( PC3 );
  sei();
  srand(time(NULL));
  
  while (1)
  {
    printf("Welcome to the four-letter word quiz!");
  
    while (stage == 1) 
    {
      writeString("CAT?");
    }

    printf("\n1 - Next category\n2 - Select category \nCategories: animal (ANML), human (HMAN), country (CTRY), thing (THNG)");

    while (stage == 2)
    {
      writeString(categories[cat_id]);
    }

    strcpy(puzzle.category, categories[cat_id]);
    strcpy(puzzle.word, words[cat_id][rand() % 28]);
    puzzle.attempts = 0;

    hideConsonants(puzzle.word, visible_word);

    button1_index = setIndex(1, visible_word);
    button2_index = setIndex(2, visible_word);
    button3_index = setIndex(3, visible_word);
    
    while (stage == 3) 
    {
      writeString(visible_word);
    }
  }

  return 0;
}
