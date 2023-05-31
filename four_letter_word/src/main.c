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

struct puzzle {
  char category[5];
  char word[5];
  int attempts;
  int time;
};

int stage = 1;
int cat_id = 0;
char* secret_word;
char visible_word[5];
int button1_index = 0;
int button2_index = 0;
int button3_index = 0;
uint16_t adcValue = 0;

char* categories[4] = {"ANML", "HMAN", "CTRY", "THNG"};
char* words[4][28] = {
  {"DUCK", "BEAR", "LION", "FROG", "FISH", "BIRD", "DEER", "WORM", "DOVE", "WASP", "TUNA", "PUMA", "CROW", "SWAN",
  "DODO", "FLEA", "GOAT", "CRAB", "MOLE", "TOAD", "SEAL", "GNAT", "HARE", "KIWI", "MOTH", "MULE", "SLUG", "WOLF"},
  {"PAUL", "GREG", "MARY", "MARC", "KATY", "NOAH", "JAKE", "ALEX", "CODY", "JANE", "OTIS", "MAYA", "LEAH", "JOSH" 
  "LIAM", "EMMA", "KATE", "LUNA", "JACK", "OWEN", "JOHN", "LUKE", "ELLA", "LILY", "RYAN", "ADAM", "ANNA", "ELLA"},
  {"IRAN", "PERU", "CUBA", "CHAD", "FIJI", "LAOS", "TOGO", "IRAQ", "BELG", "ENGL", "IREL", "NETH", "DENM", "SWED",
  "NORW", "FINL", "GERM", "FRAN", "SPAI", "PORT", "ITAL", "MEXI", "CANA", "POLA", "BRAZ", "ARGE", "JAPA", "PHIL"},
  {"BOOK", "BALL", "DESK", "DOOR", "FORK", "LAMP", "SHOE", "RING", "COMB", "SOAP", "PIPE", "VASE", "COIN", "FLAG"
  "HOOK", "OVEN", "BELL", "NOTE", "WIRE", "BIKE", "SUIT", "SOCK", "TOOL", "ROAD", "FILE", "TILE", "BOMB", "SEED"}
};

void initTimer1()
{
    TCCR1A |= _BV(WGM10); // WGM10 = 1 and WGM12 = 1 --> 8 bit Fast PWM Mode
    TCCR1B |= _BV(WGM12);

    TCCR1B |= _BV(CS12) | _BV(CS10); // CS12 = 1 and CS10 = 1 --> prescaler factor is now 1024 (= every 64 us)

    TCCR1A |= _BV(COM1B1);
}

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

  while (stage == 3) 
  {
    writeString(visible_word);
  }
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
  
  stage1();
  stage2();
  stage3();

  return 0;
}
