//
// Created by François on 28/03/2024.
//

#include <unistd.h>
#include "ams.h"
#include "amp.h"
#include "frame.h"
#include "usb.h"

int main() {
    /**
        * Programme principal de l'usb
        *
        * Ce programme lit les noms de chansons à partir d'un fichier AMP, recherche les fichiers AMS correspondants,
        * et les envoie via USB pour lecture sur la carte STM32.
        *
        * @return 0 en cas de succès, -1 en cas d'erreur.
    */
    char* filenameAMP = "Playlist_autotest.amp";
    FILE* file = initAMP(filenameAMP);
    char songname[40] = {"\0"};
    char songplaylist[4][41] = {'\0'};
    char songplaylist_txt[4][41] = {'\0'};
    char songfinal[4][41] = {'\0'};
    char songnontrouvees[4][41] = {'\0'};
    int indexnontrouvees = 0;
    int indexsongfinal = 0;

    int index = 0;
    readAMP(file, songname);
    while (songname[0]!='\0' && index < 4) {
        for (int i = 0; i < 40; i++) songplaylist[index][i] = songname[i];
        readAMP(file, songname);
        index++;
    }
    //Recherche des fichiers AMS correspondant
    FILE *fileAMStest = NULL;

    // Parcourir la liste des fichiers et essayer de les ouvrir et les stocker dans une liste de fichiers
    for (int i = 0; i < index; i++) {
        fileAMStest = NULL;
        fileAMStest = initAMP(songplaylist[i]);
        if (fileAMStest != NULL) {
            //printf("%s se trouve dans le repertoire.\n", songplaylist[i]);
            for (int j = 0; j < 40; j++) {
                songfinal[indexsongfinal][j] = songplaylist[i][j];
                songplaylist[i][j] = '\0';
            }
            indexsongfinal++;
            fclose(fileAMStest);
        } else {
            //printf("%s n'existe pas dans le repertoire.\n", songplaylist[i]);
        }
    }

    //on supprime l'extension .ams pour chercher des .txt au cas ou
    int indexsongplaylist = 0;
    for (int i = 0; i < index; i++) {
        if (songplaylist[i][0]!='\0'){
            size_t len = strlen(songplaylist[i]);
            for (int j = 0; j < 40; j++) songplaylist_txt[indexsongplaylist][j] = songplaylist[i][j];
            if (len >= 4) songplaylist_txt[indexsongplaylist][len - 4] = '\0';
            strcat(songplaylist_txt[indexsongplaylist], ".txt");
            indexsongplaylist++;
        }
    }
    //Si pas de fichier AMS cherche un fichier txt avec le meme nom pour créer un fichier AMS
    for (int i = 0; i < indexsongplaylist; i++) {
        fileAMStest = NULL;
        fileAMStest = initAMP(songplaylist_txt[i]);
        if (fileAMStest != NULL) {
            for (int j = 0; j < 40; j++) songfinal[indexsongfinal][j] = songplaylist[i][j];
            indexsongfinal++;
            createAMS(songplaylist_txt[i], songplaylist[i]);
            fclose(fileAMStest);
        } else {
            for (int j = 0; j < 40; ++j) songnontrouvees[indexnontrouvees][j] = songplaylist_txt[i][j];
            indexnontrouvees++;
        }
    }

    //Affiche le nom des musiques trouvées et celles non trouvées
    printf("Les musiques trouvees/crees sont :\n");
    for (int i = 0; i < indexsongfinal; i++) if (songfinal[i][0] != '\0')printf("%s\n", songfinal[i]);
    printf("\nLes musiques non trouvees/crees sont :\n");
    for (int i = 0; i < indexnontrouvees; i++) if (songnontrouvees[i][0] != '\0') printf("%s\n", songnontrouvees[i]);

    //Init de la connection USB
    FILE *fileAMS = NULL;
    FT_HANDLE fthandle =initUSB();
    char buffer[MAX_SIZE_LINE];
    char bufferusb[MAX_SIZE_TITLE+12+19*MAX_NUMBER_TICKS] = {'\0'};
    int indexfrm = 0;

    s_song tofrm = readAMS("");;
    while (songfinal[indexfrm][0] != '\0'){
        fileAMS = fopen(songfinal[indexfrm], "r");
        if (fileAMS == NULL) {
            perror("Error opening source file");
            return -1;
        }
        memset(bufferusb, '\0', MAX_SIZE_TITLE+12+19*MAX_NUMBER_TICKS);
        tofrm = readAMS("");
        tofrm = readAMS(songfinal[indexfrm]);;


        char title[MAX_SIZE_TITLE];
        fgets(buffer, sizeof(buffer), fileAMS);
        createInitFrame(tofrm, title);
        strcat(bufferusb, title);
        char line[MAX_SIZE_LINE];

        s_tick tick = tofrm.tickTab[0];
        long int nbtick = 0;
        fgets(buffer, sizeof(buffer), fileAMS);
        fgets(buffer, sizeof(buffer), fileAMS);
        fgets(buffer, sizeof(buffer), fileAMS);
        while (fgets(buffer, sizeof(buffer), fileAMS) != NULL) {
            tick = tofrm.tickTab[nbtick];
            createTickFrame(tick, line);
            strcat(bufferusb, line);
            nbtick++;
        }
        //printf("%s", bufferusb);
        indexfrm++;
        writeUSB(bufferusb,fthandle);
        sleep(1);
        fclose(fileAMS);
    }
    return 0;
}