#include "data.h"
#include "equipment.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int saveFile(SpaceManager *spacesManager, ClientManager *clientManager,
             ReservationManager *reservationsManager,
             EquipmentManager *equipmentManager) {
  if (!spacesManager->fileLoaded || !clientManager->fileLoaded ||
      !reservationsManager->fileLoaded || !equipmentManager->fileLoaded) {
    puts("No file has been loaded, please load a file first");
    return -1;
  }

  if (spacesManager->unsavedSpaces == 0 && clientManager->unsavedClients == 0 &&
      reservationsManager->unsavedReservations == 0 &&
      equipmentManager->unsavedEquipments == 0) {
    puts("No new data to save.");
    return 0;
  }

  FILE *file = fopen("data.bin", "wb");
  if (file == NULL) {
    printf("Could not open file data.bin for writing.\n");
    return -1;
  }

  // Spaces
  fwrite(&spacesManager->numSpaces, sizeof(int), 1, file);
  for (int i = 0; i < spacesManager->numSpaces; i++) {
    fwrite(&spacesManager->spaces[i].id, sizeof(int), 1, file);
    fwrite(spacesManager->spaces[i].name, sizeof(char), MAX_NAME_LENGTH, file);
    fwrite(spacesManager->spaces[i].type, sizeof(char), MAX_TYPE_LENGTH, file);
    fwrite(&spacesManager->spaces[i].capacity, sizeof(int), 1, file);
  }

  // Clients
  fwrite(&clientManager->numClients, sizeof(int), 1, file);
  for (int i = 0; i < clientManager->numClients; i++) {
    fwrite(&clientManager->clients[i].id, sizeof(int), 1, file);
    fwrite(clientManager->clients[i].name, sizeof(char), MAX_NAME_LENGTH, file);
    fwrite(clientManager->clients[i].phoneNumber, sizeof(char),
           MAX_PHONE_LENGTH, file);
    fwrite(clientManager->clients[i].email, sizeof(char), MAX_EMAIL_LENGTH,
           file);
    fwrite(&clientManager->clients[i].nif, sizeof(int), 1, file);
    fwrite(&clientManager->clients[i].registrationDate, sizeof(struct tm), 1,
           file);
  }

  // Reservations
  fwrite(&reservationsManager->numReservations, sizeof(int), 1, file);
  for (int i = 0; i < reservationsManager->numReservations; i++) {
    fwrite(&reservationsManager->reservations[i].id, sizeof(int), 1, file);
    fwrite(&reservationsManager->reservations[i].clientId, sizeof(int), 1,
           file);
    fwrite(&reservationsManager->reservations[i].spaceId, sizeof(int), 1, file);
    fwrite(&reservationsManager->reservations[i].reservationDate,
           sizeof(struct tm), 1, file);
    fwrite(&reservationsManager->reservations[i].duration, sizeof(int), 1,
           file);
    fwrite(&reservationsManager->reservations[i].status,
           sizeof(ReservationStatus), 1, file);
    fwrite(&reservationsManager->reservations[i].numParticipants, sizeof(int),
           1, file);
  }

  // Equipment
  fwrite(&equipmentManager->numEquipments, sizeof(int), 1, file);
  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    fwrite(&equipmentManager->equipments[i].id, sizeof(int), 1, file);
    fwrite(equipmentManager->equipments[i].name, sizeof(char),
           MAX_EQUIPMENT_NAME, file);
    fwrite(equipmentManager->equipments[i].type, sizeof(char),
           MAX_EQUIPMENT_TYPE, file);
    fwrite(&equipmentManager->equipments[i].status, sizeof(EquipmentStatus), 1,
           file);
  }

  fclose(file);
  puts("Successfully saved changes to file\n");
  clientManager->unsavedClients = 0;
  spacesManager->unsavedSpaces = 0;
  reservationsManager->unsavedReservations = 0;
  equipmentManager->unsavedEquipments = 0;

  return 0;
}

int loadFile(SpaceManager *spacesManager, ClientManager *clientManager,
             ReservationManager *reservationsManager,
             EquipmentManager *equipmentManager) {
  // Free existing memory if any
  if (spacesManager->spaces != NULL) {
    free(spacesManager->spaces);
    spacesManager->spaces = NULL;
    spacesManager->numSpaces = 0;
  }

  if (clientManager->clients != NULL) {
    free(clientManager->clients);
    clientManager->clients = NULL;
    clientManager->numClients = 0;
  }

  if (reservationsManager->reservations != NULL) {
    free(reservationsManager->reservations);
    reservationsManager->reservations = NULL;
    reservationsManager->numReservations = 0;
  }

  FILE *file = fopen("data.bin", "rb");
  if (file == NULL) {
    printf("Could not open file data.bin\n");
    return -1;
  }

  // Read the number of spaces
  fread(&spacesManager->numSpaces, sizeof(int), 1, file);
  spacesManager->spaces = malloc(spacesManager->numSpaces * sizeof(Space));

  // Read spaces data
  for (int index = 0; index < spacesManager->numSpaces; index++) {
    fread(&spacesManager->spaces[index].id, sizeof(int), 1, file);
    fread(spacesManager->spaces[index].name, sizeof(char), MAX_NAME_LENGTH,
          file);
    spacesManager->spaces[index].name[MAX_NAME_LENGTH - 1] =
        '\0'; // mark the end of the string

    fread(spacesManager->spaces[index].type, sizeof(char), MAX_TYPE_LENGTH,
          file);
    spacesManager->spaces[index].type[MAX_TYPE_LENGTH - 1] = '\0';

    fread(&spacesManager->spaces[index].capacity, sizeof(int), 1, file);
  }

  // Read clients data
  fread(&clientManager->numClients, sizeof(int), 1, file);
  clientManager->clients = malloc(clientManager->numClients * sizeof(Client));

  for (int i = 0; i < clientManager->numClients; i++) {
    fread(&clientManager->clients[i].id, sizeof(int), 1, file);
    fread(clientManager->clients[i].name, sizeof(char), MAX_NAME_LENGTH, file);
    clientManager->clients[i].name[MAX_NAME_LENGTH - 1] =
        '\0'; // Null-terminate the string

    fread(clientManager->clients[i].phoneNumber, sizeof(char), MAX_PHONE_LENGTH,
          file);
    clientManager->clients[i].phoneNumber[MAX_PHONE_LENGTH - 1] =
        '\0'; // Null-terminate the string

    fread(clientManager->clients[i].email, sizeof(char), MAX_EMAIL_LENGTH,
          file);
    clientManager->clients[i].email[MAX_EMAIL_LENGTH - 1] =
        '\0'; // Null-terminate the string

    fread(&clientManager->clients[i].nif, sizeof(int), 1, file);
    fread(&clientManager->clients[i].registrationDate, sizeof(struct tm), 1,
          file); // Load registration date
  }

  // Read reservations data
  fread(&reservationsManager->numReservations, sizeof(int), 1, file);
  reservationsManager->reservations =
      malloc(reservationsManager->numReservations * sizeof(Reservation));

  for (int i = 0; i < reservationsManager->numReservations; i++) {
    fread(&reservationsManager->reservations[i].id, sizeof(int), 1, file);
    fread(&reservationsManager->reservations[i].clientId, sizeof(int), 1, file);
    fread(&reservationsManager->reservations[i].spaceId, sizeof(int), 1, file);
    fread(&reservationsManager->reservations[i].reservationDate,
          sizeof(struct tm), 1, file);
    fread(&reservationsManager->reservations[i].duration, sizeof(int), 1, file);
    fread(&reservationsManager->reservations[i].status,
          sizeof(ReservationStatus), 1, file);
    fread(&reservationsManager->reservations[i].numParticipants, sizeof(int), 1,
          file);
  }
  // Read equipments data
  fread(&equipmentManager->numEquipments, sizeof(int), 1, file);
  equipmentManager->equipments =
      malloc(equipmentManager->numEquipments * sizeof(Equipment));

  for (int i = 0; i < equipmentManager->numEquipments; i++) {
    fread(&equipmentManager->equipments[i].id, sizeof(int), 1, file);
    fread(equipmentManager->equipments[i].name, sizeof(char),
          MAX_EQUIPMENT_NAME, file);
    equipmentManager->equipments[i].name[MAX_EQUIPMENT_NAME - 1] = '\0';

    fread(equipmentManager->equipments[i].type, sizeof(char),
          MAX_EQUIPMENT_TYPE, file);
    equipmentManager->equipments[i].type[MAX_EQUIPMENT_TYPE - 1] = '\0';

    fread(&equipmentManager->equipments[i].status, sizeof(EquipmentStatus), 1,
          file);
  }

  spacesManager->fileLoaded = 1;
  clientManager->fileLoaded = 1;
  reservationsManager->fileLoaded = 1;
  equipmentManager->fileLoaded = 1;

  spacesManager->unsavedSpaces = 0;
  clientManager->unsavedClients = 0;
  reservationsManager->unsavedReservations = 0;
  equipmentManager->unsavedEquipments = 0;

  fclose(file);
  puts("Loaded files");
  return 0;
}
