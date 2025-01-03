#include "reports.h"
#include "input.h"
#include "utilities.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
// Spaces
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

  if (spacesManager->unsavedSpaces || reservationManager->unsavedReservations) {
    puts("\nNo spaces or reservations available.");
    return;
  }

  int maxReservations = -1, minReservations = INT_MAX;
  puts("----------------------------------------");
  puts("          Most and Least Reserved Spaces                 ");
  puts("----------------------------------------");
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

  if (spacesManager->numSpaces == 0 ||
      reservationManager->numReservations == 0) {
    puts("\nNo spaces or reservations available.");
    return;
  }

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
// Clients
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

void reportClientReservations(ClientManager *clientManager,
                              ReservationManager *reservationManager,
                              SpaceManager *spaceManager) {
  if (!clientManager->fileLoaded || !reservationManager->fileLoaded ||
      !spaceManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first");
    return;
  }

  if (clientManager->unsavedClients ||
      reservationManager->unsavedReservations || spaceManager->unsavedSpaces) {
    puts("\nPlease save file first, before getting all the reports");
    return;
  }

  puts("----------------------------------------");
  puts("          Clients Reservation Report          ");
  puts("----------------------------------------");

  for (int i = 0; i < clientManager->numClients; i++) {
    int totalReservations = 0;

    // First pass to count total reservations
    for (int j = 0; j < reservationManager->numReservations; j++) {
      if (reservationManager->reservations[j].clientId ==
          clientManager->clients[i].id) {
        totalReservations++;
      }
    }

    if (totalReservations > 0) {
      printf("Client %s (ID - %d)\n", clientManager->clients[i].name,
             clientManager->clients[i].id);
      printf("Total Reservations: %d\n", totalReservations);
      puts("Spaces Used:");

      // Second pass to print reservation details
      for (int j = 0; j < reservationManager->numReservations; j++) {
        if (reservationManager->reservations[j].clientId ==
            clientManager->clients[i].id) {
          char dateBuffer[20];
          strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d",
                   &reservationManager->reservations[j].reservationDate);
          printf("- Space ID: %d, Reservation Date: %s\n",
                 reservationManager->reservations[j].spaceId, dateBuffer);
        }
      }
      puts("");
    }
  }
}
// Reservations
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

  puts("\n----------------------------------------");
  puts("           Space Occupancy Rate          ");
  puts("----------------------------------------");

  if (spaceManager->numSpaces == 0 ||
      reservationManager->numReservations == 0) {
    puts("No data available for occupancy calculation.");
    return;
  }

  // Loop through each reservation
  for (int i = 0; i < reservationManager->numReservations; i++) {
    // Skip canceled reservations
    if (reservationManager->reservations[i].status == CANCELED) {
      continue;
    }

    int reservationId = reservationManager->reservations[i].id;
    int spaceId = reservationManager->reservations[i].spaceId;
    int participants = reservationManager->reservations[i].numParticipants;

    // Find the corresponding space
    Space *currentSpace = NULL;
    for (int j = 0; j < spaceManager->numSpaces; j++) {
      if (spaceManager->spaces[j].id == spaceId) {
        currentSpace = &spaceManager->spaces[j];
        break;
      }
    }

    if (currentSpace == NULL || currentSpace->status == INACTIVE) {
      continue;
    }

    // Calculate occupancy rate for this reservation
    double occupancyRate =
        ((double)participants / currentSpace->capacity) * 100;

    // Print reservation details
    printf("\nReservation ID: %d\n", reservationId);
    printf("Space ID: %d\n", spaceId);
    printf("Space Capacity: %d people\n", currentSpace->capacity);
    printf("Number of Participants: %d\n", participants);
    printf("Occupancy Rate: %.1f%%\n", occupancyRate);
    puts("----------------------------------------");
  }
}

// Equipment
void reportMostLeastUsedEquipment(EquipmentManager *equipmentManager) {
  if (!equipmentManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first.");
    return;
  }

  if (equipmentManager->unsavedEquipments) {
    puts("\nPlease save file first");
    return;
  }

  puts("----------------------------------------");
  puts("          Most and Least Used Equipment  ");
  puts("----------------------------------------");

  if (equipmentManager->numEquipments == 0) {
    puts("\nNo equipment available.");
    return;
  }

  // Initialize usage counts array
  int usageCounts[equipmentManager->numEquipments];
  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    usageCounts[i] = 0;
    if (equipmentManager->equipments[i].equipmentStatus == RESERVED) {
      usageCounts[i] = 1; // Count current RESERVED status
    }
  }

  int maxUsage = -1;
  int minUsage = INT_MAX;
  int mostUsedIdx = -1;
  int leastUsedIdx = -1;
  int hasReserved = 0;

  // Find most and least used equipment
  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    if (equipmentManager->equipments[i].equipmentStatus == RESERVED) {
      hasReserved = 1;
      if (usageCounts[i] > maxUsage) {
        maxUsage = usageCounts[i];
        mostUsedIdx = i;
      }
      if (usageCounts[i] < minUsage) {
        minUsage = usageCounts[i];
        leastUsedIdx = i;
      }
    }
  }

  if (!hasReserved) {
    puts("\nNo equipment is currently in use (RESERVED).");
    return;
  }

  // Print results
  printf("\nMost Used Equipment: %s (Currently RESERVED)\n",
         equipmentManager->equipments[mostUsedIdx].name);
  printf("Least Used Equipment: %s (Currently RESERVED)\n",
         equipmentManager->equipments[leastUsedIdx].name);

  puts("----------------------------------------");
}
void reportEquipmentUsageRate(ReservationManager *reservationManager,
                              EquipmentManager *equipmentManager) {
  if (!equipmentManager->fileLoaded) {
    puts("\nNo file loaded, please load a file first.");
    return;
  }

  if (equipmentManager->numEquipments == 0) {
    puts("\nNo equipment available.");
    return;
  }

  if (equipmentManager->unsavedEquipments) {
    puts("\nPlease save first.");
    return;
  }
  if (reservationManager->numReservations == 0) {
    printf("No reservations available to calculate usage rate.\n");
    return;
  }
  puts("----------------------------------------");
  puts("          Equipment Usage Rate                ");
  puts("----------------------------------------");

  // Loop through all equipment
  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    int equipmentId = equipmentManager->equipments[i].id;
    int usageCount = 0;

    // Count how many reservations use this equipment
    for (int j = 0; j < reservationManager->numReservations; j++) {
      if (reservationManager->reservations[j].equipmentId == equipmentId) {
        usageCount++;
      }
    }

    // Calculate usage rate
    double usageRate =
        ((double)usageCount / reservationManager->numReservations) * 100;

    // Print the result
    printf("Equipment '%s' (ID: %d) usage rate: %.2f%%\n",
           equipmentManager->equipments[i].name, equipmentId, usageRate);
  }
}