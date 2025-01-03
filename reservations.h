#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include "utilities.h"

void viewAllReservations(ReservationManager *reservationManager,
                         SpaceManager *spaceManager,
                         ClientManager *clientManager,
                         EquipmentManager *equipmentManager);
void addNewReservation(ReservationManager *reservationManager,
                       ClientManager *clientManager,
                       SpaceManager *spacesManager,
                       EquipmentManager *equipmentManager);
void editReservation(ReservationManager *reservationManager,
                     ClientManager *clientManager, SpaceManager *spacesManager,
                     EquipmentManager *equipmentManager);
void deleteReservation(ReservationManager *reservationManager);

#endif