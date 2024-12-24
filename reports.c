#include "reports.h"
#include <stdio.h>
#include <string.h>

void reportTotalSpaces(SpaceManager *spaceManager) {
  if (!spaceManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }
  if (spaceManager->unsavedSpaces) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  puts("----------------------------------------");
  puts("          Total Number of Spaces         ");
  puts("----------------------------------------");

  printf("Saved spaces: %d\n", spaceManager->numSpaces);
  puts("----------------------------------------");
}

void reportSpacesByType(SpaceManager *spaceManager) {
  if (!spaceManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }
  if (spaceManager->unsavedSpaces) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  puts("----------------------------------------");
  puts("          Spaces by Type                 ");
  puts("----------------------------------------");
  for (int i = 0; i < spaceManager->numSpaces; i++) {
    int count = 0;

    for (int j = 0; j < spaceManager->numSpaces; j++) {
      if (strcmp(spaceManager->spaces[i].type, spaceManager->spaces[j].type) ==
          0) {
        count++;
      }
    }

    int alreadyPrinted = 0; // flag to check if type was printed already

    for (int k = 0; k < i; k++) {
      if (strcmp(spaceManager->spaces[i].type, spaceManager->spaces[k].type) ==
          0) {
        alreadyPrinted = 1;
        break;
      }
    }

    if (!alreadyPrinted) {
      printf("Type: %s, Count: %d\n", spaceManager->spaces[i].type, count);
    }
  }
}

void reportTotalClients(ClientManager *clientManager) {
  // Check if the file is loaded and no unsaved changes
  if (!clientManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }
  if (clientManager->unsavedClients) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  puts("----------------------------------------");
  puts("          Total Number of Clients        ");
  puts("----------------------------------------");
  printf("Registered clients: %d\n", clientManager->numClients);
}