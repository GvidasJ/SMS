#include "equipment.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void viewAllEquipments(EquipmentManager *equipmentManager) {
  if (equipmentManager->numEquipments == 0) {
    puts("No equipment registered.\n");
    return;
  }

  puts("\nEquipment List:");
  puts("ID | Name | Type | Status");
  puts("----------------------------------------");

  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    printf("%d | %s | %s | %s\n", equipmentManager->equipments[i].id,
           equipmentManager->equipments[i].name,
           equipmentManager->equipments[i].type,
           equipmentStatusToString(equipmentManager->equipments[i].status));
  }
  puts("");
}
