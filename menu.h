#ifndef MENU_H
#define MENU_H

#include "data.h"
#include "equipment.h"
#include "utilities.h"

void smsMenu(SpaceManager *spaceManager, ClientManager *clientManager,
             ReservationManager *reservationsManager,
             EquipmentManager *equipmentManager);
void mainMenu(SpaceManager *spaceManager, ClientManager *clientManager,
              ReservationManager *reservationsManager,
              EquipmentManager *equipmentManager);
void spacesMenu(SpaceManager *spacesManager);
void clientsMenu(ClientManager *clientsManager);
void reservationsMenu(ReservationManager *reservationsManager,
                      ClientManager *clientManager, SpaceManager *spaceManager,
                      EquipmentManager *equipmentManager);
void equipmentsMenu(EquipmentManager *equipmentManager,
                    ReservationManager *reservationManager);
void generateReports(SpaceManager *spaceManager, ClientManager *clientManager,
                     ReservationManager *reservationManager,
                     EquipmentManager *equipmentManager);

#endif