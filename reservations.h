#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include "utilities.h"

void viewAllReservations(ReservationManager *reservationManager);
void addNewReservation(ReservationManager *reservationManager, ClientManager *clientManager, SpaceManager *spacesManager);
void editReservation(ReservationManager *reservationManager, ClientManager *clientManager, SpaceManager *spacesManager);
void deleteReservation(ReservationManager*reservationManager);

#endif