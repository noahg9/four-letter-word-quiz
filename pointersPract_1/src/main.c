#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// to define two macros 
#define _MAX 5 
#define SEVENFOLD(x) (7 * (x))

// to create a function printArray 
void printArray(int array[], int size) {
  printf("Content of array:\n");
  for (int i = 0; i < size; i++) {
    printf("  Address: %p has value: %d\n", &array[i], array[i]);
  }
  printf("\n");
}

// to create a function  makeArray
void makeArray(int array[], int size) {
  for (int i = 0; i < size; i++) {
    array[i] = SEVENFOLD(i);
  }
}

int main() {
  int array[_MAX] = {0};
  printArray(array, _MAX);
  makeArray(array, _MAX);
  printArray(array, _MAX);
  return 0;
}