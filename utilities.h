#ifndef UTILITIES_H
#define UTILITIES_H

#include <time.h>

#define MAX_NAME_LENGTH 100
#define MAX_TYPE_LENGTH 100
#define MAX_PHONE_LENGTH 15
#define MAX_EMAIL_LENGTH 320

typedef enum { ACTIVE, INACTIVE } EntitiesStatus;

// Space Structs

typedef struct {
  int id;
  char name[MAX_NAME_LENGTH];
  char type[MAX_TYPE_LENGTH];
  int capacity;
  EntitiesStatus status;
} Space;

typedef struct {
  Space *spaces;
  int numSpaces;
  int unsavedSpaces;
  int fileLoaded;
  int isActive;
  int nextId;
} SpaceManager;

// Client Structs

typedef struct {
  int id;
  char name[MAX_NAME_LENGTH];
  char phoneNumber[MAX_PHONE_LENGTH];
  char email[MAX_EMAIL_LENGTH];
  int nif;
  struct tm registrationDate;
  EntitiesStatus status;
} Client;

typedef struct {
  Client *clients;
  int numClients;
  int unsavedClients;
  int fileLoaded;
  int isActive;
  int nextId;
} ClientManager;

// Reservation Structs
typedef enum { PENDING, CONFIRMED, COMPLETED, CANCELED } ReservationStatus;

typedef struct {
  int id;
  int clientId;
  int spaceId;
  int equipmentId;
  struct tm reservationDate;
  int duration;
  ReservationStatus status;
  int numParticipants;
} Reservation;

typedef struct {
  Reservation *reservations;
  int numReservations;
  int unsavedReservations;
  int fileLoaded;
  int nextId;

} ReservationManager;

void inputName(char *name, int maxLength, const char *msg);

// Equipments Struct
typedef enum { AVAILABLE, RESERVED, UNDER_MAINTENANCE } EquipmentStatus;

typedef struct {
  int id;
  char name[MAX_NAME_LENGTH];
  char type[MAX_TYPE_LENGTH];
  EquipmentStatus equipmentStatus;
  EntitiesStatus status;
} Equipment;

typedef struct {
  Equipment *equipments;
  int numEquipments;
  int unsavedEquipments;
  int fileLoaded;
  int nextId;
  int isActive;
} EquipmentManager;

// Spaces
int inputSpaceCapacity(const char *msg, int min, int max);
void inputSpaceType(char *type, int maxLength, const char *msg);

// Clients
void inputPhoneNumber(char *phoneNumber, int maxLength, const char *msg);
void inputEmail(char *email, int maxLength, const char *msg);
int inputNif(ClientManager clientManager, int excludeNif);
int inputID(const int min, int max, char *msg);

// Reservation
void inputDate(struct tm *date);

#endif
