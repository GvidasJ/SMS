#include "menu.h"
#include "clients.h"
#include "equipment.h"
#include "input.h"
#include "reports.h"
#include "reservations.h"
#include "spaces.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>

void manageEquipments() {}

void mainMenu(SpaceManager *spaceManager, ClientManager *clientManager,
              ReservationManager *reservationsManager,
              EquipmentManager *equipmentManager) {
  int choice;

  do {
    choice = getInt(1, 5,
                    "----------------------------------------"
                    "\n             Main Menu              \n"
                    "----------------------------------------\n"
                    "1. SMS Menu \n"
                    "2. Load file \n"
                    "3. Save file \n"
                    "4. Exit \n"
                    "Please select an option 1-4: ");

    switch (choice) {
    case 1:
      clearConsole();
      smsMenu(spaceManager, clientManager, reservationsManager,
              equipmentManager);
      break;
    case 2:
      clearConsole();
      loadFile(spaceManager, clientManager, reservationsManager,
               equipmentManager);
      break;
    case 3:
      clearConsole();
      saveFile(spaceManager, clientManager, reservationsManager,
               equipmentManager);
      break;
    case 4:
      saveFile(spaceManager, clientManager, reservationsManager,
               equipmentManager);
      clearConsole();
      puts("Exiting....");
      free(spaceManager->spaces);
      free(clientManager->clients);
      free(reservationsManager->reservations);
      free(equipmentManager->equipments);
      return;
    default:
      clearConsole();
      puts("Invalid choice, Please try again.\n");
      break;
    }
  } while (choice != 4);
}

void smsMenu(SpaceManager *spaceManager, ClientManager *clientManager,
             ReservationManager *reservationsManager,
             EquipmentManager *equipmentManager) {
  int choice;
  do {
    choice = getInt(1, 6,
                    "----------------------------------------"
                    "\n       Space Management System Menu       \n"
                    "----------------------------------------\n"
                    "1. Manage Spaces \n"
                    "2. Manage Clients \n"
                    "3. Manage Reservations \n"
                    "4. Manage Equipment \n"
                    "5. Generate Reports \n"
                    "6. Back\n"
                    "Please select an option 1-6: \n");

    switch (choice) {
    case 1:
      clearConsole();
      spacesMenu(spaceManager);
      break;
    case 2:
      clearConsole();
      clientsMenu(clientManager);
      break;
    case 3:
      clearConsole();
      reservationsMenu(reservationsManager, clientManager, spaceManager,
                       equipmentManager);
      break;
    case 4:
      clearConsole();
      equipmentsMenu(equipmentManager);
      break;
    case 5:
      clearConsole();
      generateReports(spaceManager, clientManager, reservationsManager,
                      equipmentManager);
      break;
    case 6:
      clearConsole();
      // puts("Exiting....");
      return;
    default:
      clearConsole();
      puts("Invalid choice. Please try again.\n");
      break;
    }
  } while (choice != 6);
}

void spacesMenu(SpaceManager *spaceManager) {
  int choice;

  do {
    choice = getInt(1, 5,
                    "----------------------------------------"
                    "\n          Space Management          \n"
                    "----------------------------------------\n"
                    "1. View All Spaces \n"
                    "2. Add New Space \n"
                    "3. Update Existing Space \n"
                    "4. Delete Space \n"
                    "5. Back to Main Menu \n"
                    "Please select an option 1-5: \n");

    switch (choice) {
    case 1:
      clearConsole();
      viewAllSpaces(spaceManager);
      break;
    case 2:
      clearConsole();
      addNewSpace(spaceManager);
      break;
    case 3:
      clearConsole();
      editSpace(spaceManager);
      break;
    case 4:
      clearConsole();
      deleteSpace(spaceManager);
      break;
    case 5:
      clearConsole();
      puts("Exiting Spaces Management Menu...\n");
      return;
    default:
      clearConsole();
      puts("Invalid choice. Please try again.\n");
      break;
    }
  } while (choice != 5);
}

void clientsMenu(ClientManager *clientManager) {
  int choice;

  do {
    choice = getInt(1, 5,
                    "----------------------------------------"
                    "\n          Client Management          \n"
                    "----------------------------------------\n"
                    "1. View All Clients \n"
                    "2. Add New Client \n"
                    "3. Update Existing Client \n"
                    "4. Delete Client \n"
                    "5. Back to Main Menu \n"
                    "Please select an option 1-5: \n");

    switch (choice) {
    case 1:
      clearConsole();
      viewAllClients(clientManager);
      break;
    case 2:
      clearConsole();
      addNewClient(clientManager);
      break;
    case 3:
      clearConsole();
      editClients(clientManager);
      break;
    case 4:
      clearConsole();
      deleteClient(clientManager);
      break;
    case 5:
      clearConsole();
      puts("Exiting Client Management Menu...\n");
      return;
    default:
      clearConsole();
      puts("Invalid choice. Please try again.\n");
      break;
    }
  } while (choice != 5);
}

void reservationsMenu(ReservationManager *reservationManager,
                      ClientManager *clientManager, SpaceManager *spacesManager,
                      EquipmentManager *equipmentManager) {
  int choice;

  do {
    choice = getInt(1, 5,
                    "----------------------------------------"
                    "\n          Reservation Management          \n"
                    "----------------------------------------\n"
                    "1. View All Reservations \n"
                    "2. Add New Reservation \n"
                    "3. Update Existing Reservation \n"
                    "4. Delete Reservation \n"
                    "5. Back to Main Menu \n"
                    "Please select an option 1-5: \n");

    switch (choice) {
    case 1:
      clearConsole();
      viewAllReservations(reservationManager, spacesManager, clientManager,
                          equipmentManager);
      break;
    case 2:
      clearConsole();
      addNewReservation(reservationManager, clientManager, spacesManager,
                        equipmentManager);
      break;
    case 3:
      clearConsole();
      editReservation(reservationManager, clientManager, spacesManager,
                      equipmentManager);
      break;
    case 4:
      clearConsole();
      deleteReservation(reservationManager);
      break;
    case 5:
      clearConsole();
      puts("Exiting Reservation Management Menu...\n");
      return;
    default:
      clearConsole();
      puts("Invalid choice. Please try again.\n");
      break;
    }
  } while (choice != 5);
}

void equipmentsMenu(EquipmentManager *manager) {
  int choice;

  do {
    choice = getInt(1, 5,
                    "----------------------------------------\n"
                    "        Equipment Management        \n"
                    "----------------------------------------\n"
                    "1. View All Equipment\n"
                    "2. Add New Equipment\n"
                    "3. Update Existing Equipment\n"
                    "4. Delete Equipment\n"
                    "5. Back to Main Menu\n"
                    "Please select an option 1-5: ");

    switch (choice) {
    case 1:
      clearConsole();
      viewAllEquipments(manager);
      break;
    case 2:
      clearConsole();
      addNewEquipment(manager);
      break;
    case 3:
      clearConsole();
      editEquipment(manager);
      break;
    case 4:
      clearConsole();
      deleteEquipment(manager);
      break;
    case 5:
      clearConsole();
      return;
    default:
      clearConsole();
      puts("Invalid choice. Please try again.\n");
    }
  } while (choice != 5);
}

void generateReports(SpaceManager *spaceManager, ClientManager *clientManager,
                     ReservationManager *reservationManager,
                     EquipmentManager *equipmentManager) {
  int choice;

  do {
    choice = getInt(0, 10,
                    "----------------------------------------"
                    "\n            Generate Reports           \n"
                    "----------------------------------------\n"
                    "1. Total Number of Spaces \n"
                    "2. Spaces by Type \n"
                    "3. Most and Least Reserved Spaces \n"
                    "4. Total Registered Clients \n"
                    "5. Client Reservation Details \n"
                    "6. Most Active Clients \n"
                    "7. Reservations by Status \n"
                    "8. Reservations by Date \n"
                    "9. Space Occupancy Rate \n"
                    "10. Most and Least Used Equipment \n"
                    "11. Equipment Usage Rate at Each Event \n"
                    "12. Resource Management Efficiency \n"
                    "0. Back to Main Menu \n"
                    "----------------------------------------\n"
                    "Please select an option 0-12: \n");

    switch (choice) {
    case 1:
      clearConsole();
      reportTotalSpaces(spaceManager);
      break;
    case 2:
      clearConsole();
      reportSpacesByType(spaceManager);
      break;
    case 3:
      clearConsole();
      reportMostLeastReservedSpaces(reservationManager, spaceManager);
      break;
    case 4:
      clearConsole();
      reportTotalClients(clientManager);
      break;
    case 5:
      clearConsole();
      reportClientReservations(clientManager, reservationManager, spaceManager);
      break;
    case 6:
      clearConsole();
      // reportActiveClients(clientManager);
      break;
    case 7:
      clearConsole();
      reportReservationsByStatus(reservationManager);
      break;
    case 8:
      clearConsole();
      reportReservationsByDate(reservationManager);
      break;
    case 9:
      clearConsole();
      reportSpaceOccupancyRate(reservationManager, spaceManager);
      break;
    case 10:
      clearConsole();
      reportMostLeastUsedEquipment(equipmentManager);
      break;
    case 11:
      clearConsole();
      // reportEquipmentUsageRate(equipmentManager);
      break;
    case 12:
      clearConsole();
      // reportResourceManagementEfficiency(equipmentManager);
      break;
    case 0:
      clearConsole();
      puts("Exiting Generate Reports Menu...\n");
      return;
    default:
      clearConsole();
      puts("Invalid choice. Please try again.\n");
      break;
    }
  } while (choice != 0);
}
