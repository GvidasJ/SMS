#include "reports.h"
#include "input.h"
#include "utilities.h"
#include <limits.h>
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

void reportMostLeastReservedSpaces(ReservationManager *reservationManager,
                                   SpaceManager *spacesManager) {
  if (!reservationManager->fileLoaded || !spacesManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first.");
    return;
  }

  if (spacesManager->numSpaces == 0 ||
      reservationManager->numReservations == 0) {
    puts("\nNo spaces or reservations available.");
    return;
  }

  int maxReservations = -1, minReservations = INT_MAX;

  // First pass: find max and min counts
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    int currentSpaceReservations = 0;
    for (int j = 0; j < reservationManager->numReservations; j++) {
      if (reservationManager->reservations[j].status != CANCELED &&
          reservationManager->reservations[j].spaceId == (i + 1)) {
        currentSpaceReservations++;
      }
    }

    if (currentSpaceReservations > maxReservations) {
      maxReservations = currentSpaceReservations;
    }
    if (currentSpaceReservations < minReservations) {
      minReservations = currentSpaceReservations;
    }
  }

  // Display results
  clearConsole();

  puts("Most reserved spaces:\n");
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    int currentSpaceReservations = 0;
    for (int j = 0; j < reservationManager->numReservations; j++) {
      if (reservationManager->reservations[j].status != CANCELED &&
          reservationManager->reservations[j].spaceId == (i + 1)) {
        currentSpaceReservations++;
      }
    }

    if (currentSpaceReservations == maxReservations) {
      printf("\nID      : %d\n", spacesManager->spaces[i].id);
      printf("Name    : %s\n", spacesManager->spaces[i].name);
      printf("Type    : %s\n", spacesManager->spaces[i].type);
      printf("Reservations: %d\n", maxReservations);
    }
  }

  puts("\nLeast reserved spaces :\n");
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    int currentSpaceReservations = 0;
    for (int j = 0; j < reservationManager->numReservations; j++) {
      if (reservationManager->reservations[j].status != CANCELED &&
          reservationManager->reservations[j].spaceId == (i + 1)) {
        currentSpaceReservations++;
      }
    }

    if (currentSpaceReservations == minReservations) {
      printf("\nID      : %d\n", spacesManager->spaces[i].id);
      printf("Name    : %s\n", spacesManager->spaces[i].name);
      printf("Type    : %s\n", spacesManager->spaces[i].type);
      printf("Reservations: %d\n", minReservations);
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
void reportReservationsByStatus(ReservationManager *reservationManager) {
  if (!reservationManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }
  if (reservationManager->unsavedReservations) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  int pending = 0, confirmed = 0, completed = 0, canceled = 0;

  puts("----------------------------------------");
  puts("        Reservations by Status          ");
  puts("----------------------------------------");

  // Count reservations by status
  for (int i = 0; i < reservationManager->numReservations; i++) {
    switch (reservationManager->reservations[i].status) {
    case PENDING:
      pending++;
      break;
    case CONFIRMED:
      confirmed++;
      break;
    case COMPLETED:
      completed++;
      break;
    case CANCELED:
      canceled++;
      break;
    }
  }

  // Display counts and percentages
  printf("Pending   : %d (%.1lf%%)\n", pending,
         (double)pending / reservationManager->numReservations * 100);
  printf("Confirmed : %d (%.1lf%%)\n", confirmed,
         (double)confirmed / reservationManager->numReservations * 100);
  printf("Completed : %d (%.1lf%%)\n", completed,
         (double)completed / reservationManager->numReservations * 100);
  printf("Canceled  : %d (%.1lf%%)\n", canceled,
         (double)canceled / reservationManager->numReservations * 100);
}

void reportReservationsByDate(ReservationManager *reservationManager) {
  if (!reservationManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }
  if (reservationManager->unsavedReservations) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  puts("----------------------------------------");
  puts("           Reservations by Date          ");
  puts("----------------------------------------");

  if (reservationManager->numReservations == 0) {
    puts("No reservations found.");
    return;
  }

  for (int i = 0; i < reservationManager->numReservations; i++) {
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d",
             &reservationManager->reservations[i].reservationDate);

    printf("Date: %s | ID: %d | Status: %s\n", dateStr,
           reservationManager->reservations[i].id,
           statusToString(reservationManager->reservations[i].status));
  }
}

// FIX THIS LATER
void reportSpaceOccupancyRate(ReservationManager *reservationManager,
                              SpaceManager *spaceManager) {

  if (!reservationManager->fileLoaded || !spaceManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }
  if (reservationManager->unsavedReservations || spaceManager->unsavedSpaces) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  puts("----------------------------------------");
  puts("           Space Occupancy Rate          ");
  puts("----------------------------------------");

  if (spaceManager->numSpaces == 0 ||
      reservationManager->numReservations == 0) {
    puts("No data available for occupancy calculation.");
    return;
  }

  // For each space, calculate occupancy
  for (int i = 0; i < spaceManager->numSpaces; i++) {
    int totalHoursReserved = 0;
    int totalReservations = 0;

    for (int j = 0; j < reservationManager->numReservations; j++) {
      if (reservationManager->reservations[j].spaceId ==
              spaceManager->spaces[i].id &&
          reservationManager->reservations[j].status == CONFIRMED) {
        totalReservations++;
        totalHoursReserved += reservationManager->reservations[j].duration;
      }
    }
    double occupancyRate =
        (double)totalReservations / spaceManager->spaces[i].capacity * 100;

    printf("Space: %s (ID: %d)\n", spaceManager->spaces[i].name,
           spaceManager->spaces[i].id);
    printf("Total Reservations: %d\n", totalReservations);
    printf("Occupancy Rate: %.1lf%%\n", occupancyRate);
    puts("----------------------------------------");
  }
}
