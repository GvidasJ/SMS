#include "equipment.h"
#include "input.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void viewAllEquipments(EquipmentManager *equipmentManager,
                       ReservationManager *reservationManager) {
  if (!equipmentManager->fileLoaded) {
    puts("No equipment loaded, please load file first.");
    return;
  }

  puts("----------------------------------------"
       "\n          Available Equipment          \n"
       "----------------------------------------\n");

  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    // Determine the status based on reservations
    Equipment *equipment = &equipmentManager->equipments[i];
    equipment->status = AVAILABLE; // Default to AVAILABLE

    for (int j = 0; j < reservationManager->numReservations; j++) {
      Reservation *reservation = &reservationManager->reservations[j];

      if (reservation->equipmentId == equipment->id &&
          (reservation->status == PENDING ||
           reservation->status == CONFIRMED)) {
        equipment->status = RESERVED;
        break; // No need to check further; the equipment is RESERVED
      }
    }
  }
  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    printf("ID      : %d\n", equipmentManager->equipments[i].id);
    printf("Name    : %s\n", equipmentManager->equipments[i].name);
    printf("Type    : %s\n", equipmentManager->equipments[i].type);
    printf("Status: %s\n",
           equipmentStatusToString(equipmentManager->equipments[i].status));
    puts("----------------------------------------\n");
  }
  puts("End of Equipments list\n");
}
void addNewEquipment(EquipmentManager *equipmentManager) {
  if (!equipmentManager->fileLoaded) {
    puts("No equipments loaded, please load file first to create a new "
         "equipment");
    return;
  }

  int newId;
  char newName[MAX_EQUIPMENT_NAME];
  char newType[MAX_EQUIPMENT_TYPE];
  int newStatus;
  Equipment *temp = NULL;
  int equipmentUsable;

  if (equipmentManager->equipments == NULL) {
    temp = malloc(sizeof(Equipment));
    equipmentManager->equipments = temp;
    newId = equipmentManager->nextId++;
  } else {
    temp = realloc(equipmentManager->equipments,
                   (equipmentManager->nextId + 1) * sizeof(Equipment));
    equipmentManager->equipments = temp;
    newId = equipmentManager->nextId++;
  }

  puts("----------------------------------------"
       "\n             Add New Equipment            \n"
       "----------------------------------------\n");

  inputName(newName, MAX_EQUIPMENT_NAME, "Enter equipment name: ");
  inputSpaceType(newType, MAX_EQUIPMENT_TYPE, "Enter equipment type: ");
  newStatus = getInt(
      0, 2, "Enter status (0: Available, 1: Reserved, 2: Under Maintenance): ");

  // Create new equipment
  Equipment newEquipment;
  newEquipment.id = newId;
  strncpy(newEquipment.name, newName, MAX_EQUIPMENT_NAME - 1);
  strncpy(newEquipment.type, newType, MAX_EQUIPMENT_TYPE - 1);
  newEquipment.name[MAX_EQUIPMENT_NAME - 1] = '\0';
  newEquipment.type[MAX_EQUIPMENT_TYPE - 1] = '\0';
  newEquipment.equipmentStatus = newStatus;
  newEquipment.status = ACTIVE;

  // Add to manager's array
  equipmentManager->equipments[equipmentManager->numEquipments] = newEquipment;
  equipmentManager->numEquipments++;
  equipmentManager->unsavedEquipments++;

  clearConsole();
  puts("\nEquipment added successfully!");
  printf("ID     : %d\n", newEquipment.id);
  printf("Name   : %s\n", newEquipment.name);
  printf("Type   : %s\n", newEquipment.type);
  printf("Status : %s\n",
         equipmentStatusToString(newEquipment.equipmentStatus));
}

void editEquipment(EquipmentManager *equipmentManager) {

  int editId;
  int foundEquipmentId = -1;
  char newName[MAX_NAME_LENGTH];
  char newType[MAX_TYPE_LENGTH];
  int newEquipmentStatus;

  if (!equipmentManager->fileLoaded || equipmentManager->numEquipments == 0 ||
      equipmentManager->equipments == NULL) {
    puts("No equipment available to edit");
    return;
  }

  puts("----------------------------------------"
       "\n          Edit Equipment              \n"
       "----------------------------------------\n");

  editId = getInt(1, equipmentManager->nextId,
                  "Enter the ID of the equipment to edit: ");

  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    if (equipmentManager->equipments[i].id == editId) {
      foundEquipmentId = i;
      break;
    }
  }

  if (foundEquipmentId == -1) {
    puts("Equipment with that ID was not found");
    return;
  }

  if (equipmentManager->equipments[foundEquipmentId].status == INACTIVE) {
    puts("Cannot edit an inactive space.");
    return;
  }

  // Display current equipment details
  puts("\nCurrent equipment details:");
  printf("ID               : %d\n",
         equipmentManager->equipments[foundEquipmentId].id);
  printf("Name             : %s\n",
         equipmentManager->equipments[foundEquipmentId].name);
  printf("Type             : %s\n",
         equipmentManager->equipments[foundEquipmentId].type);
  printf("Equipment Status : %s\n",
         equipmentStatusToString(
             equipmentManager->equipments[foundEquipmentId].equipmentStatus));
  printf("Status           : %s\n",
         statusToString(equipmentManager->equipments[foundEquipmentId].status));

  // Ask for new inputs
  inputName(newName, MAX_NAME_LENGTH,
            "Enter new equipment name or 0 to keep current: ");
  inputSpaceType(newType, MAX_TYPE_LENGTH,
                 "Enter new equipment type or 0 to keep current: ");
  newEquipmentStatus =
      getInt(0, 3,
             "Enter new status (0 to keep current, 1-Available, "
             "2-Reserved, 3-Under Maintenance): ");

  if (strcmp(newName, "0") != 0) {
    strncpy(equipmentManager->equipments[foundEquipmentId].name, newName,
            MAX_NAME_LENGTH - 1);
    equipmentManager->equipments[foundEquipmentId].name[MAX_NAME_LENGTH - 1] =
        '\0';
  }
  if (strcmp(newType, "0") != 0) {
    strncpy(equipmentManager->equipments[foundEquipmentId].type, newType,
            MAX_TYPE_LENGTH - 1);
    equipmentManager->equipments[foundEquipmentId].type[MAX_TYPE_LENGTH - 1] =
        '\0';
  }
  if (newEquipmentStatus > 0) {
    equipmentManager->equipments[foundEquipmentId].equipmentStatus =
        newEquipmentStatus - 1;
  }

  equipmentManager->unsavedEquipments++;

  clearConsole();
  puts("\nEquipment updated successfully!");
  printf("ID               : %d\n",
         equipmentManager->equipments[foundEquipmentId].id);
  printf("Name             : %s\n",
         equipmentManager->equipments[foundEquipmentId].name);
  printf("Type             : %s\n",
         equipmentManager->equipments[foundEquipmentId].type);
  printf("Equipment Status : %s\n",
         equipmentStatusToString(
             equipmentManager->equipments[foundEquipmentId].equipmentStatus));
}

void deleteEquipment(EquipmentManager *equipmentManager,
                     ReservationManager *reservationManager) {
  int deleteId;
  int foundEquipmentId = -1;

  if (!equipmentManager->fileLoaded || equipmentManager->numEquipments == 0 ||
      equipmentManager->equipments == NULL) {
    puts("No equipment available to delete");
    return;
  }

  puts("----------------------------------------"
       "\n            Delete Equipment             \n"
       "----------------------------------------\n");

  deleteId =
      getInt(1, equipmentManager->nextId, "Enter equipment ID to delete: ");

  // finding the id of equipment
  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    if (equipmentManager->equipments[i].id == deleteId) {
      foundEquipmentId = i;
      break;
    }
  }

  if (foundEquipmentId == -1) {
    puts("Equipment with that ID was not found");
    return;
  }

  for (int i = 0; i < reservationManager->numReservations; i++) {
    if (reservationManager->reservations[i].equipmentId == deleteId) {
      ReservationStatus status = reservationManager->reservations[i].status;

      if (status == PENDING || status == CONFIRMED) {
        puts("Cannot delete equipment with active or pending reservations. "
             "Please cancel or complete the reservations first.");
        return;
      }
    }
  }
  equipmentManager->numEquipments--;
  equipmentManager->unsavedEquipments++;

  if (equipmentManager->numEquipments == 0) {
    // if no equipments left
    free(equipmentManager->equipments);
    equipmentManager->equipments = NULL;
  } else {
    Equipment *temp =
        realloc(equipmentManager->equipments,
                equipmentManager->numEquipments * sizeof(Equipment));
    equipmentManager->equipments = temp;
  }

  clearConsole();
  puts("Equipment deleted successfully");
}