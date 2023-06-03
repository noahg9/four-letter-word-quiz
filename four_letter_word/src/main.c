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
#define MAX_CATEGORIES 6
#define MAX_WORDS 28
#define MAX_CATEGORY_LENGTH 5
#define MAX_WORD_LENGTH 5

typedef struct {
  char category[MAX_CATEGORY_LENGTH];
  char word[MAX_WORD_LENGTH];
  int attempts;
  int time;
} PUZZLE;

int stage = 1;
int cat_id = 0;
char visible_word[MAX_WORD_LENGTH];
int button1_index = 0;
int button2_index = 0;
int button3_index = 0;
uint16_t adc_value = 0;

volatile int seed_time = 0;
volatile int verify_time = 0;
volatile int seed_set = 0;
volatile int button_pressed = 0;
volatile int cat_selected = 0;
volatile int game_active = 0;
volatile int game_won = 0;

PUZZLE puzzle;

char categories[MAX_CATEGORIES][MAX_CATEGORY_LENGTH] = {"ANML", "HMAN", "CTRY", "THNG", "CLUB", "COMP"};
char words[MAX_CATEGORIES][MAX_WORDS][MAX_WORD_LENGTH] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER", "WORM", "DOVE", "WASP", "TUNA", "PUMA", "CROW", "SWAN",
  "DODO", "FLEA", "GOAT", "CRAB", "MOLE", "TOAD", "SEAL", "GNAT", "HARE", "KIWI", "MOTH", "MULE", "SLUG", "WOLF"},
  {"PAUL", "GREG", "MARY", "MARC", "KATY", "NOAH", "JAKE", "ALEX", "CODY", "JANE", "OTIS", "MAYA", "LEAH", "JOSH",
  "LIAM", "EMMA", "KATE", "LUNA", "JACK", "OWEN", "JOHN", "LUKE", "ELLA", "LILY", "RYAN", "ADAM", "ANNA", "ELLA"},
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO", "IRAQ", "BELG", "ENGL", "IREL", "NETH", "DENM", "SWED",
  "NORW", "FINL", "GERM", "FRAN", "SPAI", "PORT", "ITAL", "MEXI", "CANA", "POLA", "BRAZ", "ARGE", "JAPA", "PHIL"},
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE", "RING", "COMB", "SOAP", "PIPE", "VASE", "COIN", "FLAG",
  "HOOK", "OVEN", "BELL", "NOTE", "WIRE", "BIKE", "SUIT", "SOCK", "TOOL", "ROAD", "FILE", "TILE", "BOMB", "SEED"},
  {"CHEL", "MANU", "MANC", "ARSE", "TOTT", "LIVE", "EVER", "BARC", "REAL", "ATLE", "BAYE", "DORT", "JUVE", "MILA",
  "INTE", "VALE", "ANDE", "ANTW", "AJAX", "NAPO", "FENE", "GALA", "BESI", "SEVI", "ROMA", "LAZI", "BURN", "BRUG"},
  {"NIKE", "APPL", "MICR", "ADID", "TESL", "META", "VERS", "AMAZ", "GUCC", "ANDR", "LENO", "SAMS", "WALM", "VISA",
  "FORD", "NETF", "INTE", "ALPH", "TOYO", "MCDO", "JPMO", "PFIZ", "DISN", "UBER", "AIRB", "SONY", "HOND", "CISC"}
};

// Run every time TCNT0 equals the value in the OCRA register (249)
ISR(TIMER0_COMPA_vect)
{
  if (!seed_set)
  {
    seed_time++;  // Increment seed time (milliseconds)
  }
}

ISR(TIMER1_COMPA_vect)
{
  if (game_active)
  {
    puzzle.time++;  // Increment game time (seconds)
    verify_time++;  // Increment verify time (seconds)
  }
}

ISR( PCINT1_vect )
{
  if (!button_pressed) {
    if (buttonPushed(1))
    {
      button_pressed = 1;
      _delay_ms(500);
    }
  } else
  if (!cat_selected) {
    if (buttonPushed(1))
    {
      cat_id++;
      _delay_ms(500);
      if (cat_id == MAX_CATEGORIES) 
      {
        cat_id = 0;
      }
    }
    if (buttonPushed(2))
    {
      cat_selected = 1;
      _delay_ms(500);
      game_active = 1; // Activates ISR for timer 1 and buttons for game
    }
  } else
  if (game_active) {
    if (buttonPushed(1)) 
    {
      nextChar(button1_index, visible_word);
      verify_time = 0;
      lightUpAllLeds();
      lightDownOneLed(button1_index);
      _delay_ms(500);
    } 
    if (buttonPushed(2) && button2_index != 4) 
    {
      nextChar(button2_index, visible_word);
      verify_time = 0;
      lightUpAllLeds();
      lightDownOneLed(button2_index);
      _delay_ms(500);
    }
    if (buttonPushed(3) && button3_index != 4) 
    {
      nextChar(button3_index, visible_word);
      verify_time = 0;
      lightUpAllLeds();
      lightDownOneLed(button3_index);
      _delay_ms(500);    
    }
  } else
  if (game_won) {
    if (buttonPushed(1)) {
      game_won = 0;
      _delay_ms(500);
    }
  }
}

ISR(ADC_vect)
{
  adc_value = ADC;
}

void hideConsonants(const char *word, char *visible_word) 
{
  for (int i = 0; i < MAX_WORD_LENGTH-1; i++) 
  {
    if (word[i] == 'A' || word[i] == 'E' || word[i] == 'I' || word[i] == 'O' 
    || word[i] == 'U' || word[i] == visible_word[i])
    {
      visible_word[i] = word[i];
    } 
    else
    {
        visible_word[i] = '_';
    }
  }
  visible_word[MAX_WORD_LENGTH-1] = '\0'; // Add null terminator
}

int setIndex(int button, char* visible_word) 
{
  int cons_count = 0;
  for (int i = 0; i < MAX_WORD_LENGTH-1; i++) 
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
  return MAX_WORD_LENGTH-1;
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
  playTone(C5, 1000);
}

void setup()
{
  initUSART();
  initDisplay();
  initADC();
  enableAllLeds();
  lightUpAllLeds();
  enableAllButtons();

  PCICR |= _BV( PCIE1 );
  PCMSK1 |= _BV( PC1 ) | _BV( PC2 ) | _BV( PC3 );
  sei();

  initTimer0();
  initTimer1();

  button_pressed = 0;
  cat_selected = 0;
  game_active = 0;
  game_won = 0;
  cat_id = 0;
  verify_time = time(NULL);
}

int main() 
{
  while (1)
  {
    setup();  

    printf("\nWelcome to the four-letter word quiz.");
    printf("\nButton 1 - Begin");
  
    while (!button_pressed) 
    {
      writeString("CAT?");
    }

    srand(seed_time); // Set random with seed as the milliseconds until button 1 is pressed

    printf("\nButton 1 - Next category");
    printf("\nButton 2 - Select current category");
    printf("\nCategories:");
    printf("\n- Animal (ANML)");
    printf("\n- Human (HMAN)");
    printf("\n- Country (CTRY)");
    printf("\n- Thing (THNG)");
    printf("\n- Football club (CLUB)");
    printf("\n- Company (COMP)");

    while (!cat_selected)
    {
      writeString(categories[cat_id]);
    }

    strcpy(puzzle.category, categories[cat_id]);
    strcpy(puzzle.word, words[cat_id][rand() % MAX_WORDS]);
    puzzle.attempts = 0;
    puzzle.time = 0;

    hideConsonants(puzzle.word, visible_word);

    button1_index = setIndex(1, visible_word);
    button2_index = setIndex(2, visible_word);
    button3_index = setIndex(3, visible_word);

    printf("\nButton 1 - Edit first underscore");
    printf("\nButton 2 - Edit second underscore (if exists)");
    printf("\nButton 3 - Edit third underscore (if exists)");

    while (game_active) 
    {
      writeString(visible_word);
      if (verify_time >= 60) {
        if (strcmp(visible_word, puzzle.word) == 0) {
          game_active = 0;
          game_won = 1;
        }
        else 
        {
          verify_time = 0;
          puzzle.attempts++;
          incorrectSound();
          printf("\n%s is incorrent.", visible_word);
          printf("\nCurrent time: %d seconds", puzzle.time);
          printf("\nCurrent attempts: %d", puzzle.attempts);   
          hideConsonants(puzzle.word, visible_word);
        }
      }
    }
    int minutes = puzzle.time / 60;
    int seconds = puzzle.time - minutes*60;
    printf("\nIn category %s the word %s was guessed in %d tries and a total time of %dmin and %dsec.",
    puzzle.category, puzzle.word, puzzle.attempts, minutes, seconds);
    printf("\nButton 1 - Play again");
    correctSound();
    
    while (game_won) {
      writeString("YES!");
    }
  }

  return 0;
}
