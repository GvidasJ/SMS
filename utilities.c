#include "utilities.h"
#include "input.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputSpaceType(char *type, int maxLength, const char *msg) {
  do {
    readString(type, maxLength, msg);
    if (strlen(type) == 0) {
      puts("Type cannot be empty. Please try again.");
    } else {
      capitalizeWords(type);
    }
  } while (strlen(type) == 0);
}

int inputSpaceCapacity(const char *msg, int min, int max) {
  return getInt(min, max, msg);
}

void inputName(char *name, int maxLength, const char *msg) {
  do {
    readString(name, maxLength, msg);
    if (strlen(name) == 0) {
      puts("Name cannot be empty. Please try again.");
    } else {
      capitalizeWords(name);
    }
  } while (strlen(name) == 0);
}

void inputPhoneNumber(char *phoneNumber, int maxLength, const char *msg) {
  int valid;

  do {
    valid = 1;
    readString(phoneNumber, maxLength, msg);
    // Check if phone number is empty
    if (strlen(phoneNumber) == 0) {
      puts("Phone Number cannot be empty. Please try again.");
      valid = 0;
      continue;
    }

    // Check if starts with +
    if (phoneNumber[0] != '+') {
      puts("Phone number must start with +");
      valid = 0;
      continue;
    }

    // Check length (including the + symbol)
    if (strlen(phoneNumber) != 13) {
      puts("Phone number must contain 12 digits");
      valid = 0;
      continue;
    }

    // Check if remaining characters are digits
    for (int i = 4; phoneNumber[i] != '\0'; i++) {
      if (phoneNumber[i] < '0' || phoneNumber[i] > '9') {
        puts("Phone number must contain only digits after country code");
        valid = 0;
        break;
      }
    }
  } while (!valid);
}
void inputEmail(char *email, int maxLength, const char *msg) {
  int valid;
  do {
    valid = 1;
    readString(email, maxLength, msg);

    // Check if email is empty
    if (strlen(email) == 0) {
      puts("Email cannot be empty, please try again");
      valid = 0;
      continue;
    }

    // Check for @ symbol
    char *atSymbol = strchr(email, '@');
    if (atSymbol == NULL) {
      puts("Invalid email format, must contain @");
      valid = 0;
      continue;
    }

    // Check for dot after @
    char *dot = strchr(atSymbol, '.');
    if (dot == NULL) {
      puts("Invalid email format, must contain . after @.");
      valid = 0;
      continue;
    }

  } while (!valid);
}

int inputNif(ClientManager clientManager, int excludeNif) {
  int i;
  int check;
  int nif;

  do {
    check = 1; // Assume valid until proven otherwise
    nif = getInt(100000000, 999999999, "Insert NIF number: ");

    for (i = 0; i < clientManager.numClients; i++) {
      // Skip checking against the excluded NIF (e.g., the current client's NIF)
      if (nif == clientManager.clients[i].nif && nif != excludeNif) {
        check = 0;
        printf("NIF already exists!\n");
        break;
      }
    }
  } while (check == 0);

  return nif;
}

int inputID(const int min, int max, char *msg) { return getInt(min, max, msg); }

void inputDate(struct tm *date) {
  int year, month, day;

  year = getInt(2024, 2100, "Enter year in a range of 2024-2100: ");
  month = getInt(1, 12, "Enter month 1-12: ");
  day = getInt(1, 31, "Enter day 1-31:");

  date->tm_year = year - 1900;
  date->tm_mon = month - 1;
  date->tm_mday = day;
}
