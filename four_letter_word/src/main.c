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
#include <buzzer.h>

// Constants
#define CATEGORY_AMOUNT 6
#define WORD_AMOUNT 28
#define CATEGORY_LENGTH 5
#define WORD_LENGTH 5

// PUZZLE struct
typedef struct {
  char category[CATEGORY_LENGTH];
  char word[WORD_LENGTH];
  int attempts;
  int time;
} PUZZLE;

// Global variables
int cat_id;
char visible_word[WORD_LENGTH];
int button1_index;
int button2_index;
int button3_index;
int seed_time;
int verify_time;
int seed_set;
int button_pressed;
int cat_selected;
int game_active;
int game_won;
PUZZLE *riddle; // Declared dynamically

// Array of categories as string
char categories[CATEGORY_AMOUNT][CATEGORY_LENGTH] = {"ANML", "HMAN", "CTRY", "THNG", "CLUB", "COMP"};

// Array of 6 category arrays with 28 words each
char words[CATEGORY_AMOUNT][WORD_AMOUNT][WORD_LENGTH] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER", "WORM", "DOVE", "WASP", "TUNA", "PUMA", "CROW", "SWAN",
  "DODO", "FLEA", "GOAT", "CRAB", "MOLE", "TOAD", "SEAL", "GNAT", "HARE", "KIWI", "MOTH", "MULE", "SLUG", "WOLF"},  // Animal
  {"PAUL", "GREG", "MARY", "MARC", "KATY", "NOAH", "JAKE", "ALEX", "CODY", "JANE", "OTIS", "MAYA", "LEAH", "JOSH",
  "LIAM", "EMMA", "KATE", "LUNA", "JACK", "OWEN", "JOHN", "LUKE", "ELLA", "LILY", "RYAN", "ADAM", "ANNA", "ELLA"},  // Human
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO", "IRAQ", "BELG", "ENGL", "IREL", "NETH", "DENM", "SWED",
  "NORW", "FINL", "GERM", "FRAN", "SPAI", "PORT", "ITAL", "MEXI", "CANA", "POLA", "BRAZ", "ARGE", "JAPA", "PHIL"},  // Country
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE", "RING", "COMB", "SOAP", "PIPE", "VASE", "COIN", "FLAG",
  "HOOK", "OVEN", "BELL", "NOTE", "WIRE", "BIKE", "SUIT", "SOCK", "TOOL", "ROAD", "FILE", "TILE", "BOMB", "SEED"},  // Thing
  {"CHEL", "MANU", "MANC", "ARSE", "TOTT", "LIVE", "EVER", "BARC", "REAL", "ATLE", "BAYE", "DORT", "JUVE", "MILA",
  "INTE", "VALE", "ANDE", "ANTW", "AJAX", "NAPO", "FENE", "GALA", "BESI", "SEVI", "ROMA", "LAZI", "BURN", "BRUG"},  // Football club
  {"NIKE", "APPL", "MICR", "ADID", "TESL", "META", "VERS", "AMAZ", "GUCC", "ANDR", "LENO", "SAMS", "WALM", "VISA",
  "FORD", "NETF", "INTE", "ALPH", "TOYO", "MCDO", "JPMO", "PFIZ", "DISN", "UBER", "AIRB", "SONY", "HOND", "CISC"}   // Company
};

// Runs every time TCNT0 equals the value in the OCRA register (249)
ISR(TIMER0_COMPA_vect)
{
  if (!seed_set)  // While no seed is set to srand
  {
    seed_time++;  // Increment seed time (milliseconds)
  }
}

ISR(TIMER1_COMPA_vect)
{
  if (game_active)  // While game is active
  {
    riddle->time++;  // Increment game time (seconds)
    if (verify_time < 60)
    {
      verify_time++;  // Increment timer to verify word (seconds)
    }
  }
}

ISR( PCINT1_vect )  // Actions associated with each button for different conditions
{
  if (!button_pressed)  // Until button 1 is first pressed
  {
    if (buttonPushed(1))
    {
      button_pressed = 1;
      _delay_ms(500);
    }
  } 

  else if (!cat_selected)  // Until category has been selected
  {
    if (buttonPushed(1))
    {
      cat_id++; // Get next category
      _delay_ms(500);
      if (cat_id == CATEGORY_AMOUNT)  // Loop through categories
      {
        cat_id = 0;
      }
    }
    if (buttonPushed(2))
    {
      cat_selected = 1;
      fadeInAllLeds(1000);
      lightUpAllLeds();
      game_active = 1; // Activates ISR for timer 1 and buttons for game
    }
  } 

  else if (game_active)  // While game is being played
  {  
    if (buttonPushed(1)) 
    {
      nextChar(button1_index, visible_word);  // Increment first consonant
      verify_time = 0;  // Reset 1-minute timer
      lightUpAllLeds();
      lightDownOneLed(button1_index); // Turn off led in same position of index incremented
      _delay_ms(500);
    } 
    if (buttonPushed(2) && button2_index != 4)
    {
      nextChar(button2_index, visible_word);  // Increment second consonant (if exists)
      verify_time = 0;
      lightUpAllLeds();
      lightDownOneLed(button2_index);
      _delay_ms(500);
    }
    if (buttonPushed(3) && button3_index != 4)
    {
      nextChar(button3_index, visible_word);  // Increment third consonant (if exists)
      verify_time = 0;
      lightUpAllLeds();
      lightDownOneLed(button3_index);
      _delay_ms(500);    
    }
  } 

  else if (game_won) // After game has been won and until button 1 is pressed
  {
    if (buttonPushed(1))
    {
      game_won = 0;
      _delay_ms(500);
    }
  }
}

void initTimer0()
{
  TCCR0A |= _BV(WGM01); // Set timer 0 to CTC mode
  TCCR0B |= _BV(CS01) | _BV(CS00);  // Set prescaler to 64
  // Set compare match register to generate interrupt every 1 millisecond
  OCR0A = 249; // Assuming a 16MHz clock and prescaler of 64 (16,000,000 / 64 / 1000 - 1)
  TIMSK0 |= _BV(OCIE0A);  // Enable compare match A interrupt
}

void initTimer1()
{
  TCCR1B |= _BV(WGM12); // Set timer 1 to CTC mode
  TCCR1B |= _BV(CS12) | _BV(CS10);  // Set prescaler to 1024
  OCR1A = 15624; // Set OCR0A for 1-second interval
  TIMSK1 |= _BV(OCIE1A);  // Enable compare match A interrupt
}

void hideConsonants(const char *word, char *visible_word) // Set variable as copy of word with consonants replaced with underscores
{
  for (int i = 0; i < WORD_LENGTH-1; i++) // Loop through each letter
  {
    if (word[i] == 'A' || word[i] == 'E' || word[i] == 'I' || word[i] == 'O' 
    || word[i] == 'U' || word[i] == visible_word[i])  // If index is vowel
    {
      visible_word[i] = word[i];  // Set index the same as the word
    } 
    else  // If index is consonant
    {
        visible_word[i] = '_';  // Set underscore on the index
    }
  }
  visible_word[WORD_LENGTH-1] = '\0'; // Add null terminator
}

int setIndex(int button, char* visible_word)  // Set index of letter to be controlled by each button
{
  int cons_count = 0;
  for (int i = 0; i < WORD_LENGTH-1; i++) 
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
  return WORD_LENGTH-1; // Make button unusable if there are no consonants left
}

void nextChar(int index, char *visible_word)  // Set next character on display
{
  if (visible_word[index] == '_' || visible_word[index] == 'Z') // Loop through alphabet beginning at A
  {
    visible_word[index] = 'A';
  }
  else
  {
    visible_word[index] = visible_word[index] + 1;  // Increment character
  }
}

void correctSound()
{
  enableBuzzer();
  float frequencies[] = {C5, D5, E5, F5, G5, A5, B5, C6};
  for ( int note = 0; note < 8; note++ )
  {
    playTone( frequencies[note], 150 );
    _delay_ms( 150 ); 
  }
 }

void incorrectSound()
{
  enableBuzzer();
  playTone(C5, 1000); // Long low tune
}

void setup()  // Initialize all necessary features when program is run
{
  initUSART();
  initDisplay();
  enableAllLeds();
  enableAllButtons();
  initTimer0();
  initTimer1();
  PCICR |= _BV(PCIE1);
  PCMSK1 |= _BV(BUTTON1) | _BV(BUTTON2) | _BV(BUTTON3);
  sei();  // Activate interrupt system globally
}

void reset()  // Reset before starting a new game which allows playing multiple games in one session
{
  button_pressed = 0;
  cat_selected = 0;
  game_active = 0;
  game_won = 0;
  cat_id = 0;
  verify_time = 0;
  lightDownAllLeds();
  free(riddle); // Free dynamically allocated memory for riddle struct
  riddle = NULL;
}

void start()
{
  reset();

  // Using array to avoid using multiple printf statements for efficient memory usage
  char start_print[] = "FOUR LETTER WORD QUIZ\n"
  "\nYou will be presented with a 4-letter word of which only the vowels are shown."
  "\nThe aim is to guess the right word in as few attempts as possible.\n"
  "\nButton 1 - Ready\n";
  printf("%s", start_print);  // Output general information about the application

  while (!button_pressed) 
  {
    writeString("CAT?");
  }
}

void catSelection()
{
  srand(seed_time); // Set seed as milliseconds elapsed before button 1 is pressed at start

  // Using array to avoid using multiple printf statements for efficient memory usage
  char cat_print[] = "\nSelect a category:"
  "\n- Animal (ANML)"
  "\n- Human (HMAN)"
  "\n- Country (CTRY)"
  "\n- Thing (THNG)"
  "\n- Football club (CLUB)"
  "\n- Company (COMP)\n"
  "\nButton 1 - Next category"
  "\nButton 2 - Select category\n";
  printf("%s", cat_print);

  while (!cat_selected)
  {
    writeString(categories[cat_id]);
  }

  riddle = malloc(sizeof(PUZZLE));  // Dynamically allocate memory to the riddle struct

  // Set values to the riddle struct fields
  strcpy(riddle->category, categories[cat_id]);
  strcpy(riddle->word, words[cat_id][rand() % WORD_AMOUNT]);  // Set random word of chosen category as the value of word
  riddle->attempts = 0;
  riddle->time = 0;

  hideConsonants(riddle->word, visible_word);

  // Allocate indexes for each button to control (index set to -1 if button is not needed)
  button1_index = setIndex(1, visible_word);
  button2_index = setIndex(2, visible_word);
  button3_index = setIndex(3, visible_word);
}

void game()
{
  // Using array to avoid using multiple printf statements for efficient memory usage
  char game_print[] = "\nButton 1 - Edit first underscore"
  "\nButton 2 - Edit second underscore (if exists)"
  "\nButton 3 - Edit third underscore (if exists)"
  "\nTo confirm the word, do not press any button for one minute.\n";
  printf("%s", game_print);

  while (game_active) // Game loop
  {
    writeString(visible_word);
    if (verify_time >= 60)  // When no button has been pressed for a minute
    {
      if (strcmp(visible_word, riddle->word) == 0)  // If the guess matches the word
      {
        game_active = 0;  // Ends game
        game_won = 1;
      }
      else  // If the guess does not match the word
      {
        verify_time = 0;  // Resets timer
        riddle->attempts++; // Increments number of attempts
        incorrectSound();
        hideConsonants(riddle->word, visible_word); // Resets only the incorrect letters
      }
    }
  }
}

void success()
{
  int minutes = riddle->time / 60;
  int seconds = riddle->time - minutes*60;
  printf("\nIn category %s the word %s was guessed in %d tries and a total time of %dmin and %dsec.",
  riddle->category, riddle->word, riddle->attempts, minutes, seconds);  // Output game information
  printf("\nButton 1 - Play again\n");
  correctSound();
  
  while (game_won)
  {
    writeString("YES!");
  }

  printf("\n\n");
}

int main() 
{
  setup();

  while (1)
  {
    start();
    catSelection();
    game();
    success();
  }

  return 0;
}
