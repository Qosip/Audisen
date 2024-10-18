//
// Created by ISEN on 13/02/2024.
//

#include "define.h"
#include "frame.h"
#include "ftd2xx.h"
#include "usb.h"


FT_HANDLE initUSB(){
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;

    // ouverture du périphérique USB
    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK) {
        printf("Erreur d'ouverture %d\n", (int)ftStatus);
        return NULL;
    }

    // Configuration
    ftStatus = FT_SetBaudRate(ftHandle, 9600);
    if (ftStatus != FT_OK) {
        printf("Erreur du set baudrate %d\n", (int)ftStatus);
        FT_Close(ftHandle);
        return NULL;
    }
    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE); // 8 bits de données, 1 bit de stop, pas de parité
    if (ftStatus != FT_OK) {
        printf("Erreur de la mise en place des charactéristiques de FT %d\n", (int)ftStatus);
        FT_Close(ftHandle);
        return NULL;
    }

    return ftHandle;
}

void closeUSB(FT_HANDLE ftHandle) {
    if (ftHandle != NULL) FT_Close(ftHandle);
    else printf("Erreur Handle NULL\n");
}


void writeUSB(char* frame, FT_HANDLE ftHandle){
    DWORD bytesWritten;
    FT_STATUS ftStatus;

    // Vérification du handle
    if (ftHandle == NULL) {
        printf("Erreur mauvais handle\n");
        return;
    }

    // Écriture des données sur le périphérique USB
    ftStatus = FT_Write(ftHandle, frame, strlen(frame), &bytesWritten);
    if (ftStatus != FT_OK) {
        printf("Erreur de FT_Write %d\n", (int)ftStatus);
        return;
    }

    printf("Donnees ecrites\n");
}