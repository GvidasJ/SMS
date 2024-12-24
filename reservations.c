#include "reservations.h"
#include "data.h"
#include "input.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void viewAllReservations(ReservationManager *reservationManager) {
  if (!reservationManager->fileLoaded) {
    puts("No reservations loaded, please load file first.");
    return;
  }

  puts("----------------------------------------"
       "\n          Available Reservations          \n"
       "----------------------------------------\n");

  for (int i = 0; i < reservationManager->numReservations; i++) {
    printf("Reservation ID  : %d\n", reservationManager->reservations[i].id);
    printf("Client ID       : %d\n",
           reservationManager->reservations[i].clientId);
    printf("Space ID        : %d\n",
           reservationManager->reservations[i].spaceId);

    // format and display date
    char dateBuffer[20];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M",
             &reservationManager->reservations[i].reservationDate);
    printf("Reservation Date: %s\n", dateBuffer);

    printf("Duration        : %d hours\n",
           reservationManager->reservations[i].duration);
    printf("Status          : %s\n",
           statusToString(reservationManager->reservations[i].status));
    printf("Participants    : %d\n",
           reservationManager->reservations[i].numParticipants);
    puts("----------------------------------------\n");
  }
  puts("End of reservations list\n");
}

void addNewReservation(ReservationManager *reservationManager,
                       ClientManager *clientManager,
                       SpaceManager *spacesManager) {
  if (!reservationManager->fileLoaded) {
    puts("No reservations loaded, please load file first to create a new "
         "reservation");
    return;
  }

  int newId;
  int findClientId;
  int findSpaceId;
  struct tm newReservationDate;
  int newDuration;
  ReservationStatus newStatus;
  int newNumParticipants;
  Reservation *temp = NULL;

  // If memory is empty, allocate space for the first element
  if (reservationManager->reservations == NULL) {
    temp = malloc(sizeof(Reservation));
    reservationManager->reservations = temp;
    newId = 1;
  } else {
    Reservation *temp = realloc(reservationManager->reservations,
                                (reservationManager->numReservations + 1) *
                                    sizeof(Reservation));
    reservationManager->reservations = temp;

    newId = reservationManager->numReservations + 1;
  }

  puts("----------------------------------------"
       "\n             Add New Reservation            \n"
       "----------------------------------------\n");

  viewAllSpaces(spacesManager);

  // Input details for new reservation
  findClientId = inputID(1, clientManager->numClients, "Enter client ID: ");
  findSpaceId = inputID(1, spacesManager->numSpaces, "Enter space ID: ");
  newDuration = getInt(1, 12, "Enter duration (hours): ");
  newStatus = getInt(
      0, 3,
      "Enter status (0: Pending, 1: Confirmed, 2: Completed, 3: Canceled): ");
  newNumParticipants = getInt(0, 10000, "Enter number of participants: ");

  Space selectedSpace =
      spacesManager
          ->spaces[findSpaceId - 1]; // Assuming 1-based indexing for space IDs
  do {
    newNumParticipants = getInt(0, 10000, "Enter number of participants: ");
    if (newNumParticipants > selectedSpace.capacity) {
      printf("\n: Number of participants exceeds the space capacity of %d\n",
             selectedSpace.capacity);
    }
  } while (newNumParticipants > selectedSpace.capacity);

  // Get current date
  time_t t = time(NULL);
  struct tm *currentTime = localtime(&t);
  // Create new reservation
  Reservation newReservation;
  newReservation.id = newId;
  newReservation.clientId = findClientId;
  newReservation.spaceId = findSpaceId;
  newReservation.reservationDate = *currentTime;
  newReservation.duration = newDuration;
  newReservation.status = newStatus;
  newReservation.numParticipants = newNumParticipants;

  // Add to reservationManager's array
  reservationManager->reservations[reservationManager->numReservations] =
      newReservation;
  reservationManager->numReservations++;
  reservationManager->unsavedReservations++;

  clearConsole();
  puts("\nReservation added successfully!");
  printf("Reservation ID  : %d\n", newReservation.id);
  printf("Client ID       : %d\n", newReservation.clientId);
  printf("Space ID        : %d\n", newReservation.spaceId);
  printf("Reservation Date: %02d/%02d/%d %02d:%02d\n",
         newReservation.reservationDate.tm_mday,
         newReservation.reservationDate.tm_mon + 1,
         newReservation.reservationDate.tm_year + 1900,
         newReservation.reservationDate.tm_hour,
         newReservation.reservationDate.tm_min);
  printf("Duration        : %d hours\n", newReservation.duration);
  printf("Status          : %s\n", statusToString(newReservation.status));
  printf("Participants    : %d\n", newReservation.numParticipants);
}

void deleteReservation(ReservationManager *reservationManager) {
  int deleteId;
  int foundReservationId = -1;

  if (!reservationManager->fileLoaded ||
      reservationManager->numReservations == 0 ||
      reservationManager->reservations == NULL) {
    puts("No reservations available to delete");
    return;
  }

  puts("----------------------------------------"
       "\n            Delete Reservation             \n"
       "----------------------------------------\n");

  deleteId = getInt(1, reservationManager->numReservations,
                    "Enter the ID of the reservation to delete: ");

  // finding the id
  for (int i = 0; i < reservationManager->numReservations; i++) {
    if (reservationManager->reservations[i].id == deleteId) {
      foundReservationId = i;
      break;
    }
  }

  if (foundReservationId == -1) {
    puts("Reservation with that ID was not found");
    return;
  }

  // Shift reservations and change ID numbers to make them in order
  for (int i = foundReservationId; i < reservationManager->numReservations - 1;
       i++) {
    reservationManager->reservations[i] =
        reservationManager->reservations[i + 1];
    reservationManager->reservations[i].id = i + 1;
  }

  reservationManager->numReservations--;
  reservationManager->unsavedReservations++;

  if (reservationManager->numReservations == 0) {
    // if no reservations left
    free(reservationManager->reservations);
    reservationManager->reservations = NULL;
  } else {
    Reservation *temp =
        realloc(reservationManager->reservations,
                reservationManager->numReservations * sizeof(Reservation));
    reservationManager->reservations = temp;
  }

  clearConsole();
  puts("Reservation deleted successfully");
}

void editReservation(ReservationManager *reservationManager,
                     ClientManager *clientManager,
                     SpaceManager *spacesManager) {
  int editId;
  int foundReservationId = -1;
  int findClientId;
  int findSpaceId;
  struct tm newReservationDate;
  int newDuration;
  ReservationStatus newStatus;
  int newNumParticipants;

  if (!reservationManager->fileLoaded ||
      reservationManager->numReservations == 0 ||
      reservationManager->reservations == NULL) {
    puts("No reservations available to edit");
    return;
  }

  puts("----------------------------------------"
       "\n             Edit Reservation              \n"
       "----------------------------------------\n");

  editId = getInt(1, reservationManager->numReservations,
                  "Enter the ID of the reservation to edit: ");

  // Finding the reservation to edit
  for (int i = 0; i < reservationManager->numReservations; i++) {
    if (reservationManager->reservations[i].id == editId) {
      foundReservationId = i;
      break;
    }
  }

  if (foundReservationId == -1) {
    puts("Reservation with that ID was not found");
    return;
  }

  // Display current reservation details
  puts("\nCurrent reservation details:");
  printf("ID               : %d\n",
         reservationManager->reservations[foundReservationId].id);
  printf("Client ID        : %d\n",
         reservationManager->reservations[foundReservationId].clientId);
  printf("Space ID         : %d\n",
         reservationManager->reservations[foundReservationId].spaceId);
  printf("Reservation Date : %s",
         asctime(&reservationManager->reservations[foundReservationId]
                      .reservationDate));
  printf("Duration         : %d\n",
         reservationManager->reservations[foundReservationId].duration);
  printf("Status           : %d\n",
         reservationManager->reservations[foundReservationId].status);
  printf("Participants     : %d\n\n",
         reservationManager->reservations[foundReservationId].numParticipants);

  findClientId = getInt(0, clientManager->numClients,
                        "Enter new Client ID or 0 to keep current: ");
  findSpaceId = getInt(0, spacesManager->numSpaces,
                       "Enter new Space ID or 0 to keep current: ");
  newDuration =
      getInt(0, 12, "Enter new duration (in hours) or 0 to keep current: ");
  newStatus = (ReservationStatus)getInt(
      0, 3,
      "Enter new status (0 = PENDING, 1 = CONFIRMED, 2 = COMPLETED, 3 = "
      "CANCELED) or -1 to keep current: ");
  newNumParticipants = getInt(
      0, 10000, "Enter new number of participants or 0 to keep current: ");

  if (findClientId > 0) {
    reservationManager->reservations[foundReservationId].clientId =
        findClientId;
  }
  if (findSpaceId > 0) {
    reservationManager->reservations[foundReservationId].spaceId = findSpaceId;
  }
  if (newDuration > 0) {
    reservationManager->reservations[foundReservationId].duration = newDuration;
  }
  if (newStatus != -1) {
    reservationManager->reservations[foundReservationId].status = newStatus;
  }
  if (newNumParticipants > 0) {
    reservationManager->reservations[foundReservationId].numParticipants =
        newNumParticipants;
  }
  if (newReservationDate.tm_year != 0) {
    reservationManager->reservations[foundReservationId].reservationDate =
        newReservationDate;
  }

  reservationManager->unsavedReservations++;

  clearConsole();
  puts("\nReservation updated successfully!");
  printf("ID               : %d\n",
         reservationManager->reservations[foundReservationId].id);
  printf("Client ID        : %d\n",
         reservationManager->reservations[foundReservationId].clientId);
  printf("Space ID         : %d\n",
         reservationManager->reservations[foundReservationId].spaceId);
  printf("Duration         : %d\n",
         reservationManager->reservations[foundReservationId].duration);
  printf("Status           : %d\n",
         reservationManager->reservations[foundReservationId].status);
  printf("Participants     : %d\n",
         reservationManager->reservations[foundReservationId].numParticipants);
}
