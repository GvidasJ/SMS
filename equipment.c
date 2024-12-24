#include "equipment.h"
#include "input.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void viewAllEquipments(EquipmentManager *equipmentManager) {
  if (!equipmentManager->fileLoaded) {
    puts("No equipment loaded, please load file first.");
    return;
  }

  puts("----------------------------------------"
       "\n          Available Equipment          \n"
       "----------------------------------------\n");

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

  if (equipmentManager->equipments == NULL) {
    temp = malloc(sizeof(Equipment));
    equipmentManager->equipments = temp;
    newId = 1;
  } else {
    temp = realloc(equipmentManager->equipments,
                   (equipmentManager->numEquipments + 1) * sizeof(Equipment));
    equipmentManager->equipments = temp;
    newId = equipmentManager->numEquipments + 1;
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
  newEquipment.status = newStatus;

  // Add to manager's array
  equipmentManager->equipments[equipmentManager->numEquipments] = newEquipment;
  equipmentManager->numEquipments++;
  equipmentManager->unsavedEquipments++;

  clearConsole();
  puts("\nEquipment added successfully!");
  printf("ID     : %d\n", newEquipment.id);
  printf("Name   : %s\n", newEquipment.name);
  printf("Type   : %s\n", newEquipment.type);
  printf("Status : %s\n", equipmentStatusToString(newEquipment.status));
}

void editEquipment(EquipmentManager *equipmentManager) {
  if (!equipmentManager->fileLoaded || equipmentManager->numEquipments == 0 ||
      equipmentManager->equipments == NULL) {
    puts("No equipment available to edit");
    return;
  }

  int editId;
  int foundEquipmentId = -1;
  char newName[MAX_EQUIPMENT_NAME];
  char newType[MAX_EQUIPMENT_TYPE];
  int newStatus;

  puts("----------------------------------------"
       "\n          Edit Equipment              \n"
       "----------------------------------------\n");

  viewAllEquipments(equipmentManager);
  editId = getInt(1, equipmentManager->numEquipments,
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
  inputName(newName, MAX_EQUIPMENT_NAME,
            "Enter new equipment name or 0 to keep current: ");
  inputSpaceType(newType, MAX_EQUIPMENT_TYPE,
                 "Enter new equipment type or 0 to keep current: ");
  newStatus = getInt(0, 3,
                     "Enter new status (0 to keep current, 1-Available, "
                     "2-Reserved, 3-Under Maintenance): ");

  if (strcmp(newName, "0") != 0) {
    strncpy(equipmentManager->equipments[foundEquipmentId].name, newName,
            MAX_EQUIPMENT_NAME - 1);
    equipmentManager->equipments[foundEquipmentId]
        .name[MAX_EQUIPMENT_NAME - 1] = '\0';
  }
  if (strcmp(newType, "0") != 0) {
    strncpy(equipmentManager->equipments[foundEquipmentId].type, newType,
            MAX_EQUIPMENT_TYPE - 1);
    equipmentManager->equipments[foundEquipmentId]
        .type[MAX_EQUIPMENT_TYPE - 1] = '\0';
  }
  if (newStatus > 0) {
    equipmentManager->equipments[foundEquipmentId].status = newStatus - 1;
  }

  equipmentManager->unsavedEquipments++;

  clearConsole();
  puts("\nEquipment updated successfully!");
  viewAllEquipments(equipmentManager);
}

void deleteEquipment(EquipmentManager *equipmentManager) {
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

  deleteId = getInt(1, equipmentManager->numEquipments,
                    "Enter equipment ID to delete: ");
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
  // Shift equipments and change ID numbers to make them in order
  for (int i = foundEquipmentId; i < equipmentManager->numEquipments - 1; i++) {
    equipmentManager->equipments[i] = equipmentManager->equipments[i + 1];
    equipmentManager->equipments[i].id = i + 1;
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