#ifndef DATA_H
#define DATA_H

#include "utilities.h"
#include "clients.h"
#include "spaces.h"
#include "reservations.h"

int loadFile(SpaceManager *manager, ClientManager *clientManager, ReservationManager *reservationsManager);
int saveFile(SpaceManager *manager, ClientManager *clientManager, ReservationManager *reservationsManager);

#endif