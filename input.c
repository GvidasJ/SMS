#include "input.h"
#include "utilities.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearConsole() { system("cls"); }

void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void capitalizeWords(char *str) {
  str[0] = toupper(str[0]); // capitalize first letter

  for (int i = 1; str[i] != '\0'; i++) {
    if (str[i - 1] == ' ') {    // if previous char was space
      str[i] = toupper(str[i]); // then capitalize this letter
    }
  }
}

// get integer input within a specific range
int getInt(int minValue, int maxValue, const char *msg) {
  int value;
  printf("%s", msg);
  while (scanf("%d", &value) != 1 || value < minValue || value > maxValue) {
    puts(INVALID_VALUE);
    clearInputBuffer();
    printf("%s", msg);
  }
  clearInputBuffer();
  return value;
}

// get string
void readString(char *string, unsigned int size, const char *msg) {
  printf("%s", msg);
  if (fgets(string, size, stdin) != NULL) {
    unsigned int last = strlen(string) - 1;
    if (string[last] == '\n') {
      string[last] = '\0'; // end a string
    } else {
      clearInputBuffer();
    }
  }
}

char getChar(const char *msg) {
  char value;
  printf("%s", msg);
  value = getchar();
  clearInputBuffer();
  return value;
}

const char *statusToString(ReservationStatus status) {
  switch (status) {
    case PENDING:   return "Pending";
    case CONFIRMED: return "Confirmed";
    case COMPLETED: return "Completed";
    case CANCELED:  return "Canceled";
    default:        return "Unknown";
  }
}