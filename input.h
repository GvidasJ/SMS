#ifndef INPUT_H
#define INPUT_H

#define INVALID_VALUE "The value entered is invalid"

#include "utilities.h"

void clearConsole();
void clearInputBuffer();
void capitalizeWords(char *str);

// inputs
int getInt(int minValue, int maxValue, const char *msg);
void readString(char *string, unsigned int size, const char *msg);
char getChar(const char *msg);
const char *statusToString(ReservationStatus status);
const char *equipmentStatusToString(EquipmentStatus status);

#endif /* INPUT_H */
