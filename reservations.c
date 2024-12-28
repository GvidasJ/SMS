#include "reservations.h"
#include "data.h"
#include "input.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void viewAllReservations(ReservationManager *reservationManager,
                         SpaceManager *spacesManager) {
  if (!reservationManager->fileLoaded) {
    puts("No reservations loaded, please load file first.");
    return;
  }

  puts("----------------------------------------"
       "\n          Available Reservations          \n"
       "----------------------------------------\n");

  for (int i = 0; i < reservationManager->numReservations; i++) {
    int spaceId = reservationManager->reservations[i].spaceId;
    int spaceExists = 0;

    // Check if the spaceId exists in spacesManager
    for (int j = 0; j < spacesManager->numSpaces; j++) {
      if (spacesManager->spaces[j].id == spaceId) {
        spaceExists = 1;
        break;
      }
    }

    printf("Reservation ID  : %d\n", reservationManager->reservations[i].id);
    printf("Client ID       : %d\n",
           reservationManager->reservations[i].clientId);
    if (!spaceExists) {
      printf("Space ID        : %d (Invalid - Does not exist)\n", spaceId);
    } else {
      printf("Space ID        : %d (Valid)\n", spaceId);
    }
    // format and display dateR
    char dateBuffer[20];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d",
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

  int newId, findClientId, findSpaceId, newDuration, newNumParticipants;
  struct tm newReservationDate;
  ReservationStatus newStatus;
  Reservation *temp = NULL;
  Space *selectedSpace = NULL;

  // If memory is empty, allocate space for the first element
  if (reservationManager->reservations == NULL) {
    temp = malloc(sizeof(Reservation));
    reservationManager->reservations = temp;
    newId = reservationManager->nextId++;
  } else {
    temp = realloc(reservationManager->reservations,
                   (reservationManager->nextId + 1) * sizeof(Reservation));

    reservationManager->reservations = temp;

    newId = reservationManager->nextId++;
  }
  // Input details for new reservation
  puts("----------------------------------------"
       "\n             Add New Reservation            \n"
       "----------------------------------------\n");

  do {
    findSpaceId = inputID(1, INT_MAX, "Enter space ID: ");
    selectedSpace = NULL;

    // Search for the space with the specified ID
    for (int i = 0; i < spacesManager->numSpaces; i++) {
      if (spacesManager->spaces[i].id == findSpaceId) {
        selectedSpace = &spacesManager->spaces[i];
        break;
      }
    }

    if (selectedSpace == NULL) {
      printf("Space with ID %d does not exist. Please try again.\n",
             findSpaceId);
      continue;
    }

    if (selectedSpace->status == INACTIVE) {
      printf("Space ID %d is in-active with another reservation. Please "
             "select another space.\n",
             findSpaceId);
    } else {
      break;
    }
  } while (1); // Repeat until space witch is non-active has been selected

  findClientId = inputID(1, clientManager->numClients, "Enter client ID: ");

  puts("Enter reservation date: ");
  inputDate(&newReservationDate);

  newDuration = getInt(1, 12, "Enter duration (hours): ");
  newStatus = getInt(
      0, 3,
      "Enter status (0: Pending, 1: Confirmed, 2: Completed, 3: Canceled): ");
  do {
    newNumParticipants = getInt(0, 10000, "Enter number of participants: ");
    if (newNumParticipants > selectedSpace->capacity) {
      printf("\nNumber of participants exceeds the space capacity of %d\n",
             selectedSpace->capacity);
    }
  } while (newNumParticipants > selectedSpace->capacity);

  // Create new reservation
  Reservation newReservation;
  newReservation.id = newId;
  newReservation.clientId = findClientId;
  newReservation.spaceId = findSpaceId;
  newReservation.reservationDate = newReservationDate;
  newReservation.duration = newDuration;
  newReservation.status = newStatus;
  newReservation.numParticipants = newNumParticipants;

  printf("ReservationStatus %d", newReservation.status);

  if (newStatus == PENDING || newStatus == CONFIRMED) {
    selectedSpace->status = INACTIVE;
  } else {
    selectedSpace->status = ACTIVE;
  }

  // Add to reservationManager's array
  reservationManager->reservations[reservationManager->numReservations] =
      newReservation;
  reservationManager->numReservations++;
  reservationManager->unsavedReservations++;

  spacesManager->unsavedSpaces++;

  // selectedClient->status = ACTIVE;
  // clientManager->unsavedClients++;

  clearConsole();
  puts("\nReservation added successfully!");
  printf("Reservation ID  : %d\n", newReservation.id);
  printf("Client ID       : %d\n", newReservation.clientId);
  printf("Space ID        : %d\n", newReservation.spaceId);
  printf("Reservation Date: %02d/%02d/%d\n",
         newReservation.reservationDate.tm_mday,
         newReservation.reservationDate.tm_mon + 1,
         newReservation.reservationDate.tm_year + 1900);
  printf("Duration        : %d hours\n", newReservation.duration);
  printf("Status          : %s\n", statusToString(newReservation.status));
  printf("Participants    : %d\n", newReservation.numParticipants);
}

void deleteReservation(ReservationManager *reservationManager,
                       SpaceManager *spacesManager) {
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

  for (int i = foundReservationId; i < reservationManager->numReservations - 1;
       i++) {
    reservationManager->reservations[i] =
        reservationManager->reservations[i + 1];
  }

  // Check reservation status before allowing deletion
  ReservationStatus status =
      reservationManager->reservations[foundReservationId].status;
  if (status == PENDING || status == CONFIRMED) {
    if (status == PENDING) {
      puts("Cannot delete a pending reservation, please cancel or confirm it "
           "first");
    } else {
      puts("Cannot delete a confirmed reservation, please wait until it's "
           "completed or canceled");
    }
    return;
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
  int newDate;
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

  Space *space =
      &spacesManager->spaces
           [reservationManager->reservations[foundReservationId].spaceId - 1];

  char dateBuffer[20];
  strftime(
      dateBuffer, sizeof(dateBuffer), "%Y-%m-%d",
      &reservationManager->reservations[foundReservationId].reservationDate);
  printf("Reservation Date : %s\n", dateBuffer);

  // Display current reservation details
  puts("\nCurrent reservation details:");
  printf("ID               : %d\n",
         reservationManager->reservations[foundReservationId].id);
  printf("Client ID        : %d\n",
         reservationManager->reservations[foundReservationId].clientId);
  printf("Space ID         : %d\n",
         reservationManager->reservations[foundReservationId].spaceId);
  printf("Duration         : %d\n",
         reservationManager->reservations[foundReservationId].duration);
  printf("Status           : %s\n",
         statusToString(
             reservationManager->reservations[foundReservationId].status));
  printf("Participants     : %d\n\n",
         reservationManager->reservations[foundReservationId].numParticipants);

  findClientId = getInt(0, clientManager->numClients,
                        "Enter new Client ID or 0 to keep current: ");
  findSpaceId = getInt(0, spacesManager->numSpaces,
                       "Enter new Space ID or 0 to keep current: ");

  newDate = getInt(0, 1, "Enter 1 for new date or 0 to keep current: ");
  if (newDate != 0) {
    inputDate(&newReservationDate);
    reservationManager->reservations[foundReservationId].reservationDate =
        newReservationDate;
  }

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
    spacesManager->unsavedSpaces++;
  }
  if (findSpaceId > 0) {
    reservationManager->reservations[foundReservationId].spaceId = findSpaceId;
    spacesManager->unsavedSpaces++;
  }
  if (newDuration > 0) {
    reservationManager->reservations[foundReservationId].duration = newDuration;
    spacesManager->unsavedSpaces++;
  }
  if (newStatus >= 0) {
    reservationManager->reservations[foundReservationId].status = newStatus;
    printf("NEw status of the space: %d", newStatus);
    space->status =
        (newStatus == CANCELED || newStatus == COMPLETED) ? ACTIVE : INACTIVE;
    spacesManager->unsavedSpaces++;
  }
  if (newNumParticipants > 0) {
    reservationManager->reservations[foundReservationId].numParticipants =
        newNumParticipants;
    spacesManager->unsavedSpaces++;
  }
  clearConsole();
  char updatedDateBuffer[20];
  strftime(
      updatedDateBuffer, sizeof(updatedDateBuffer), "%Y-%m-%d",
      &reservationManager->reservations[foundReservationId].reservationDate);

  puts("\nReservation updated successfully!");
  printf("ID          : %d\n",
         reservationManager->reservations[foundReservationId].id);
  printf("Client ID   : %d\n",
         reservationManager->reservations[foundReservationId].clientId);
  printf("Space ID    : %d\n",
         reservationManager->reservations[foundReservationId].spaceId);

  printf("Date        : %s\n", updatedDateBuffer);

  printf("Duration    : %d\n",
         reservationManager->reservations[foundReservationId].duration);
  printf("Status      : %s\n",
         statusToString(
             reservationManager->reservations[foundReservationId].status));
  printf("Participants: %d\n",
         reservationManager->reservations[foundReservationId].numParticipants);
}
