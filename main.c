#include "data.h"
#include "equipment.h"
#include "input.h"
#include "menu.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int choice;
  SpaceManager spaceManager = {NULL, 0, 0, 0};
  ClientManager clientManager = {NULL, 0, 0, 0};
  ReservationManager reservationManager = {NULL, 0, 0, 0};
  EquipmentManager equipmentManager = {NULL, 0, 0, 0};
  clearConsole();
  mainMenu(&spaceManager, &clientManager, &reservationManager,
           &equipmentManager);
  return 0;
}