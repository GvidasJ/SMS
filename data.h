#ifndef DATA_H
#define DATA_H

#include "clients.h"
#include "equipment.h"
#include "reservations.h"
#include "spaces.h"
#include "utilities.h"

int loadFile(SpaceManager *manager, ClientManager *clientManager,
             ReservationManager *reservationsManager,
             EquipmentManager *equipmentManager);

int saveFile(SpaceManager *manager, ClientManager *clientManager,
             ReservationManager *reservationsManager,
             EquipmentManager *equipmentManager);

#endif