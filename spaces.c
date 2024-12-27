#include "spaces.h"
#include "data.h"
#include "input.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void viewAllSpaces(SpaceManager *spacesManager) {
  if (!spacesManager->fileLoaded) {
    puts("No spaces loaded, please load file first.");
    return;
  }

  puts("----------------------------------------"
       "\n          Available Spaces          \n"
       "----------------------------------------\n");

  for (int i = 0; i < spacesManager->numSpaces; i++) {
    if (spacesManager->spaces[i].id != -1) { // Only show non-deleted spaces
      printf("ID      : %d\n", spacesManager->spaces[i].id);
      printf("Name    : %s\n", spacesManager->spaces[i].name);
      printf("Type    : %s\n", spacesManager->spaces[i].type);
      printf("Capacity: %d\n", spacesManager->spaces[i].capacity);
      printf("Status  : %s\n",
             spacesManager->spaces[i].status == ACTIVE ? "ACTIVE" : "INACTIVE");

      puts("----------------------------------------\n");
    }
  }
  puts("End of spaces list\n");
}

void addNewSpace(SpaceManager *spacesManager) {
  if (!spacesManager->fileLoaded) {
    puts("No spaces loaded, please load file first to create a new space");
    return;
  }

  int newId;
  char newName[MAX_NAME_LENGTH];
  char newType[MAX_TYPE_LENGTH];
  int newCapacity;
  Space *temp = NULL;

  // Find the smallest available ID
  newId = 1; // Start with the first possible ID
  for (int i = 1; i <= spacesManager->numSpaces + 1; i++) {
    int idExists = 0;
    for (int j = 0; j < spacesManager->numSpaces; j++) {
      if (spacesManager->spaces[j].id == i) {
        idExists = 1;
        break;
      }
    }
    if (!idExists) {
      newId = i; // Assign the first unused ID
      break;
    }
  }

  // Check for empty slots first
  int emptySlot = -1;
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    if (spacesManager->spaces[i].id == -1) {
      emptySlot = i;
      break;
    }
  }

  if (emptySlot == -1) {
    temp = realloc(spacesManager->spaces,
                   (spacesManager->numSpaces + 1) * sizeof(Space));
    if (temp == NULL) {
      puts("Memory allocation failed!");
      return;
    }
    spacesManager->spaces = temp;
    emptySlot = spacesManager->numSpaces;
    spacesManager->numSpaces++;
  }

  puts("----------------------------------------"
       "\n             Add New Space            \n"
       "----------------------------------------\n");

  inputName(newName, MAX_NAME_LENGTH, "Enter space name: ");
  inputSpaceType(newType, MAX_TYPE_LENGTH, "Enter space type: ");
  newCapacity = inputSpaceCapacity("Enter space capacity: ", 1, 10000);

  // Create new space
  Space newSpace;
  newSpace.id = newId;
  strncpy(newSpace.name, newName, MAX_NAME_LENGTH - 1);
  strncpy(newSpace.type, newType, MAX_TYPE_LENGTH - 1);
  newSpace.name[MAX_NAME_LENGTH - 1] = '\0';
  newSpace.type[MAX_TYPE_LENGTH - 1] = '\0';
  newSpace.capacity = newCapacity;

  // Add to spacesManager's array
  spacesManager->spaces[emptySlot] = newSpace;
  // spacesManager->numSpaces++;
  spacesManager->unsavedSpaces++;

  clearConsole();
  puts("\nSpace added successfully!");
  printf("ID      : %d\n", newSpace.id);
  printf("Name    : %s\n", newSpace.name);
  printf("Type    : %s\n", newSpace.type);
  printf("Capacity: %d\n", newSpace.capacity);
}

void deleteSpace(SpaceManager *spacesManager) {
  int deleteId;
  int foundSpaceId = -1;

  if (!spacesManager->fileLoaded || spacesManager->numSpaces == 0 ||
      spacesManager->spaces == NULL) {
    puts("No spaces available to delete");
    return;
  }

  puts("----------------------------------------"
       "\n            Delete Space             \n"
       "----------------------------------------\n");

  deleteId = getInt(1, spacesManager->numSpaces,
                    "Enter the ID of the space to delete: ");

  // Finding the ID
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    if (spacesManager->spaces[i].id == deleteId) {
      foundSpaceId = i;
      break;
    }
  }

  if (foundSpaceId == -1) {
    puts("Space with that ID was not found");
    return;
  }

  // Shift spaces and change ID numbers to make them in order
  // for (int i = foundSpaceId; i < spacesManager->numSpaces - 1; i++) {
  //  spacesManager->spaces[i] = spacesManager->spaces[i + 1];
  // spacesManager->spaces[i].id = i + 1;
  //}

  // Check if the space is active
  if (spacesManager->spaces[foundSpaceId].status == ACTIVE) {
    puts("Cannot delete space because its status is ACTIVE");
    return;
  }

  spacesManager->spaces[foundSpaceId].id = -1;
  spacesManager->unsavedSpaces++;

  if (spacesManager->numSpaces == 0) {
    free(spacesManager->spaces);
    spacesManager->spaces = NULL;
  } else {
    Space *temp = realloc(spacesManager->spaces,
                          spacesManager->numSpaces * sizeof(Space));
    spacesManager->spaces = temp;
  }

  clearConsole();
  puts("Space deleted successfully");
}

void editSpace(SpaceManager *spacesManager) {
  int editId;
  int foundSpaceId = -1;
  char newName[MAX_NAME_LENGTH];
  char newType[MAX_TYPE_LENGTH];
  int newCapacity;

  if (!spacesManager->fileLoaded || spacesManager->numSpaces == 0 ||
      spacesManager->spaces == NULL) {
    puts("No spaces available to edit");
    return;
  }

  puts("----------------------------------------"
       "\n             Edit Space              \n"
       "----------------------------------------\n");

  editId = getInt(1, spacesManager->numSpaces,
                  "Enter the ID of the space to edit: ");

  // Finding the space to edit
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    if (spacesManager->spaces[i].id == editId) {
      foundSpaceId = i;
      break;
    }
  }

  if (foundSpaceId == -1) {
    puts("Space with that ID was not found");
    return;
  }

  if (spacesManager->spaces[foundSpaceId].status == ACTIVE) {
    puts("Cannot edit an active space.");
    return;
  }

  // Display current values
  puts("\nCurrent space details:");
  printf("ID      : %d\n", spacesManager->spaces[foundSpaceId].id);
  printf("Name    : %s\n", spacesManager->spaces[foundSpaceId].name);
  printf("Type    : %s\n", spacesManager->spaces[foundSpaceId].type);
  printf("Capacity: %d\n\n", spacesManager->spaces[foundSpaceId].capacity);

  inputName(newName, MAX_NAME_LENGTH,
            "Enter new space name or 0 to keep current: ");
  inputSpaceType(newType, MAX_TYPE_LENGTH,
                 "Enter new space type or 0 to keep current: ");
  newCapacity = inputSpaceCapacity(
      "Enter new space capacity or 0 to keep current: ", 0, 10000);

  if (strcmp(newName, "0") != 0) {
    strncpy(spacesManager->spaces[foundSpaceId].name, newName,
            MAX_NAME_LENGTH - 1);
    spacesManager->spaces[foundSpaceId].name[MAX_NAME_LENGTH - 1] = '\0';
  }
  if (strcmp(newType, "0") != 0) { // Fixed condition
    strncpy(spacesManager->spaces[foundSpaceId].type, newType,
            MAX_TYPE_LENGTH - 1);
    spacesManager->spaces[foundSpaceId].type[MAX_TYPE_LENGTH - 1] = '\0';
  }
  if (newCapacity > 0) {
    spacesManager->spaces[foundSpaceId].capacity = newCapacity;
  }

  spacesManager->unsavedSpaces++;

  clearConsole();
  puts("\nSpace updated successfully!");
  printf("ID      : %d\n", spacesManager->spaces[foundSpaceId].id);
  printf("Name    : %s\n", spacesManager->spaces[foundSpaceId].name);
  printf("Type    : %s\n", spacesManager->spaces[foundSpaceId].type);
  printf("Capacity: %d\n", spacesManager->spaces[foundSpaceId].capacity);
}
