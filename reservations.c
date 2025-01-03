#include "reservations.h"
#include "data.h"
#include "input.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void viewAllReservations(ReservationManager *reservationManager,
                         SpaceManager *spacesManager,
                         ClientManager *clientsManager,
                         EquipmentManager *equipmentsManager) {
  if (!reservationManager->fileLoaded) {
    puts("No reservations loaded, please load file first.");
    return;
  }

  puts("----------------------------------------"
       "\n          Available Reservations          \n"
       "----------------------------------------\n");

  for (int i = 0; i < reservationManager->numReservations; i++) {
    int spaceId = reservationManager->reservations[i].spaceId;
    int clientId = reservationManager->reservations[i].clientId;
    int equipmentId = reservationManager->reservations[i].equipmentId;
    int spaceExists = 0;
    int clientsExists = 0;
    int equipmentExists = 0;

    // Check if the spaceId exists in spacesManager
    for (int j = 0; j < spacesManager->numSpaces; j++) {
      if (spacesManager->spaces[j].id == spaceId) {
        spaceExists = 1;
        break;
      }
    }

    // Check if the clientId exists in clientsManager
    for (int j = 0; j < clientsManager->numClients; j++) {
      if (clientsManager->clients[j].id == clientId) {
        clientsExists = 1;
        break;
      }
    }

    // Check if the equipmentId exists in equipentsManager
    for (int j = 0; j < equipmentsManager->numEquipments; j++) {
      if (equipmentsManager->equipments[j].id == equipmentId) {
        equipmentExists = 1;
        break;
      }
    }

    printf("Reservation ID  : %d\n", reservationManager->reservations[i].id);
    if (!clientsExists) {
      printf("Client ID        : %d (Invalid - Does not exist)\n", clientId);
    } else {
      printf("CLient ID        : %d (Valid)\n", clientId);
    }
    if (!spaceExists) {
      printf("Space ID        : %d (Invalid - Does not exist)\n", spaceId);
    } else {
      printf("Space ID        : %d (Valid)\n", spaceId);
    }
    if (!equipmentExists) {
      printf("Equipment ID    : %d (Invalid - Does not exist)\n", equipmentId);
    } else {
      printf("Equipment ID    : %d (Valid)\n", equipmentId);
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
                       SpaceManager *spacesManager,
                       EquipmentManager *equipmentsManager) {
  if (!reservationManager->fileLoaded) {
    puts("No reservations loaded, please load file first to create a new "
         "reservation");
    return;
  }

  int newId, findClientId, findSpaceId, findEquipmentId, newDuration,
      newNumParticipants;
  struct tm newReservationDate;
  ReservationStatus newStatus;
  Reservation *temp = NULL;
  Space *selectedSpace = NULL;
  Client *selectedClient = NULL;
  Equipment *selectedEquipment = NULL;

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
    break; // Remove the status check here as we'll check date conflicts later
  } while (1);

  do {
    findClientId = inputID(1, INT_MAX, "Enter client ID: ");
    selectedClient = NULL;

    // Search for the client with the specified ID
    for (int i = 0; i < clientManager->numClients; i++) {
      if (clientManager->clients[i].id == findClientId) {
        selectedClient = &clientManager->clients[i];
        break;
      }
    }

    if (selectedClient == NULL) {
      printf("Client with ID %d does not exist. Please try again.\n",
             findClientId);
      continue;
    }

    break;
  } while (1);

  do {
    findEquipmentId = inputID(1, INT_MAX, "Enter Equipment ID: ");
    selectedEquipment = NULL;

    // Search for the equipment with the specified ID
    for (int i = 0; i < equipmentsManager->numEquipments; i++) {
      if (equipmentsManager->equipments[i].id == findEquipmentId) {
        selectedEquipment = &equipmentsManager->equipments[i];
        break;
      }
    }

    if (selectedEquipment == NULL) {
      printf("Equiment with ID %d does not exist. Please try again.\n",
             findEquipmentId);
      continue;
    }

    break;
  } while (1);

  do {
    puts("Enter reservation date: ");
    inputDate(&newReservationDate);

    int dateMatches = 0;
    for (int i = 0; i < reservationManager->numReservations; i++) {
      Reservation *existingReservation = &reservationManager->reservations[i];

      // Only check reservations for the same space
      if (existingReservation->spaceId == findSpaceId) {
        // Check if the dates match
        if (existingReservation->reservationDate.tm_year ==
                newReservationDate.tm_year &&
            existingReservation->reservationDate.tm_mon ==
                newReservationDate.tm_mon &&
            existingReservation->reservationDate.tm_mday ==
                newReservationDate.tm_mday) {
          printf(
              "Error: Space ID %d is already reserved for date %02d/%02d/%d\n",
              findSpaceId, newReservationDate.tm_mday,
              newReservationDate.tm_mon + 1, newReservationDate.tm_year + 1900);
          dateMatches = 1;
          break;
        }
      }
    }

    if (!dateMatches) {
      break; // exit loop after valid date found
    }
  } while (1);

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
  newReservation.equipmentId = findEquipmentId;
  newReservation.reservationDate = newReservationDate;
  newReservation.duration = newDuration;
  newReservation.status = newStatus;
  newReservation.numParticipants = newNumParticipants;

  printf("ReservationStatus %d", newReservation.status);

  if (newStatus == PENDING || newStatus == CONFIRMED) {
    selectedEquipment->equipmentStatus = RESERVED;

    selectedSpace->status = INACTIVE;
    selectedClient->status = INACTIVE;
    selectedEquipment->status = INACTIVE;
  } else {
    selectedSpace->status = ACTIVE;
    selectedClient->status = ACTIVE;
    selectedEquipment->status = ACTIVE;
  }

  // Add to reservationManager's array
  reservationManager->reservations[reservationManager->numReservations] =
      newReservation;
  reservationManager->numReservations++;
  reservationManager->unsavedReservations++;

  spacesManager->unsavedSpaces++;
  clientManager->unsavedClients++;
  equipmentsManager->unsavedEquipments++;

  clearConsole();
  puts("\nReservation added successfully!");
  printf("Reservation ID  : %d\n", newReservation.id);
  printf("Client ID       : %d\n", newReservation.clientId);
  printf("Space ID        : %d\n", newReservation.spaceId);
  printf("Equipment ID   : %d\n", newReservation.equipmentId);
  printf("Reservation Date: %02d/%02d/%d\n",
         newReservation.reservationDate.tm_mday,
         newReservation.reservationDate.tm_mon + 1,
         newReservation.reservationDate.tm_year + 1900);
  printf("Duration        : %d hours\n", newReservation.duration);
  printf("Status          : %s\n", statusToString(newReservation.status));
  printf("Participants    : %d\n", newReservation.numParticipants);
}

void editReservation(ReservationManager *reservationManager,
                     ClientManager *clientManager, SpaceManager *spacesManager,
                     EquipmentManager *equipmentsManager) {
  int editId;
  int foundReservationId = -1;
  int newDate;
  struct tm newReservationDate;
  int newDuration;
  ReservationStatus newStatus;
  int newNumParticipants;
  Space *spacePtr;
  Client *clientPtr;
  Equipment *equipmentPtr;

  if (!reservationManager->fileLoaded ||
      reservationManager->numReservations == 0 ||
      reservationManager->reservations == NULL) {
    puts("No reservations available to edit");
    return;
  }

  puts("----------------------------------------"
       "\n             Edit Reservation              \n"
       "----------------------------------------\n");

  editId = getInt(1, reservationManager->nextId,
                  "Enter the ID of the reservation to edit: ");

  int x = 0;
  for (int i = 0; i < reservationManager->numReservations; i++) {
    x = reservationManager->reservations[i].spaceId;
  }

  int y = 0;
  for (int i = 0; i < reservationManager->numReservations; i++) {
    y = reservationManager->reservations[i].clientId;
  }

  int z = 0;
  for (int i = 0; i < reservationManager->numReservations; i++) {
    z = reservationManager->reservations[i].equipmentId;
  }

  spacePtr = &spacesManager->spaces[x - 1];
  clientPtr = &clientManager->clients[y - 1];
  equipmentPtr = &equipmentsManager->equipments[z - 1];

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

  Client *client =
      &clientManager->clients
           [reservationManager->reservations[foundReservationId].clientId - 1];

  Equipment *equipment =
      &equipmentsManager->equipments
           [reservationManager->reservations[foundReservationId].equipmentId -
            1];

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
  printf("Equipment ID         : %d\n",
         reservationManager->reservations[foundReservationId].equipmentId);
  printf("Duration         : %d\n",
         reservationManager->reservations[foundReservationId].duration);
  printf("Status           : %s\n",
         statusToString(
             reservationManager->reservations[foundReservationId].status));
  printf("Participants     : %d\n\n",
         reservationManager->reservations[foundReservationId].numParticipants);

  // EDIT DATE
  newDate = getInt(0, 1, "Enter 1 for new date or 0 to keep current: ");
  if (newDate != 0) {
    inputDate(&newReservationDate);
    reservationManager->reservations[foundReservationId].reservationDate =
        newReservationDate;
  }

  // EDIT DURATION
  newDuration =
      getInt(0, 12, "Enter new duration (in hours) or 0 to keep current: ");
  if (newDuration > 0) {
    reservationManager->reservations[foundReservationId].duration = newDuration;
    spacesManager->unsavedSpaces++;
  }

  // EDIT STATUS
  newStatus = (ReservationStatus)getInt(
      0, 4,
      "Enter new status (0 = PENDING, 1 = CONFIRMED, "
      "2 = COMPLETED, 3 = CANCELED) or to keep current enter 4: ");
  if (newStatus == 4) {
    newStatus = reservationManager->reservations[foundReservationId].status;
  } else if (newStatus !=
             reservationManager->reservations[foundReservationId].status) {
    reservationManager->reservations[foundReservationId].status = newStatus;
    spacesManager->unsavedSpaces++;

    if (newStatus == PENDING || newStatus == CONFIRMED) {
      spacePtr->status = INACTIVE;
      clientPtr->status = INACTIVE;
      equipmentPtr->status = INACTIVE;
      equipmentPtr->equipmentStatus = RESERVED;
    } else if (newStatus == COMPLETED || newStatus == CANCELED) {
      spacePtr->status = ACTIVE;
      clientPtr->status = ACTIVE;
      equipmentPtr->status = ACTIVE;
      equipmentPtr->equipmentStatus = AVAILABLE;
    }
  }

  // EDIT NUMBER OF PARTICIPANTS
  do {
    newNumParticipants = getInt(
        0, 10000, "Enter new number of participants or 0 to keep current: ");

    if (newNumParticipants == 0) {
      break;
    }

    if (space->capacity < newNumParticipants) {
      puts("The capacity of the space is lower than the number of "
           "participants.");
    } else {
      reservationManager->reservations[foundReservationId].numParticipants =
          newNumParticipants;
      spacesManager->unsavedSpaces++;
      break;
    }
  } while (1);

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
  printf("Equipment ID: %d\n",
         reservationManager->reservations[foundReservationId].equipmentId);
  printf("Date        : %s\n", updatedDateBuffer);
  printf("Duration    : %d\n",
         reservationManager->reservations[foundReservationId].duration);
  printf("Status      : %s\n",
         statusToString(
             reservationManager->reservations[foundReservationId].status));
  printf("Participants: %d\n",
         reservationManager->reservations[foundReservationId].numParticipants);
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

  deleteId = getInt(1, reservationManager->nextId,
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
