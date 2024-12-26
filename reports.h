#ifndef REPORTS_H
#define REPORTS_H

#include "utilities.h"

//Spaces
void reportSpacesByType(SpaceManager *spaceManager);
void reportTotalSpaces(SpaceManager *spaceManager);

//Clients
void reportTotalClients(ClientManager *clientManager);
void reportClientReservations(ClientManager *clientManager,
                              ReservationManager *reservationManager,
                              SpaceManager *spaceManager);

//Reservation
void reportReservationsByStatus(ReservationManager *reservationManager);
void reportReservationsByDate(ReservationManager *reservationManager);
void reportSpaceOccupancyRate(ReservationManager *reservationManager, SpaceManager *spaceManager);
void reportMostLeastReservedSpaces(ReservationManager *reservationManager, SpaceManager *spacesManager);

//Equipment
void reportMostLeastUsedEquipment(EquipmentManager *equipmentManager);

#endif
