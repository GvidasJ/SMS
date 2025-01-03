#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#define MAX_EQUIPMENT_NAME 100
#define MAX_EQUIPMENT_TYPE 50

#include "utilities.h"

void viewAllEquipments(EquipmentManager *equipmentManager,
                       ReservationManager *reservationManager);
void addNewEquipment(EquipmentManager *equipmentManager);
void editEquipment(EquipmentManager *equipmentManager);
void deleteEquipment(EquipmentManager *equipmentManager,
                     ReservationManager *reservationManager);

#endif