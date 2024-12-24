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
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M",
             &reservationManager->reservations[i].reservationDate);

    // Get enum value as a string
    const char *status;
    if (reservationManager->reservations[i].status == 0)
      status = "Pending";
    else if (reservationManager->reservations[i].status == 1)
      status = "Confirmed";
    else if (reservationManager->reservations[i].status == 2)
      status = "Completed";
    else if (reservationManager->reservations[i].status == 3)
      status = "Canceled";
    else
      status = "Unknown";

    printf("Date: %s | ID: %d | Status: %s\n", dateStr,
           reservationManager->reservations[i].id, status);
  }
}
