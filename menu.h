#ifndef MENU_H
#define MENU_H

#include "data.h"
#include "utilities.h"

void smsMenu(SpaceManager *spaceManager, ClientManager *clientManager,
             ReservationManager *reservationsManager);
void mainMenu(SpaceManager *spaceManager, ClientManager *clientManager,
              ReservationManager *reservationsManager);
void spacesMenu(SpaceManager *spacesManager);
void clientsMenu(ClientManager *clientsManager);
void reservationsMenu(ReservationManager *reservationsManager,
                      ClientManager *clientManager, SpaceManager *spaceManager);
void generateReports(SpaceManager *spaceManager, ClientManager *clientManager,
                     ReservationManager *reservationManager);

#endif