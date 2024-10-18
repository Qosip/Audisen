#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frame.h"
#include "define.h"



void createInitFrame(s_song mySong, char* frame){
    /**
        * Crée une trame d'initialisation au format spécifié à partir des données de la chanson.
        * @param mySong La structure contenant les données de la chanson.
        * @param frame Le tableau de caractères où stocker la trame créée.
    */
    char * contenu = (char*)malloc(50 * sizeof(char));

    sprintf(contenu, "%s,%d,%d", mySong.title, mySong.tpm, mySong.nTicks);

    int resultat_xor = contenu[0];
    int i=1;
    while(contenu[i]!='\0'){
        resultat_xor ^= contenu[i];
        i++;
    }
    free(contenu);
    char hex_string[2];
    if(resultat_xor<16){
        sprintf(hex_string, "%x", resultat_xor);
        hex_string[1]=hex_string[0];
        hex_string[0]=48;
    }
    else sprintf(hex_string, "%x", resultat_xor);

    sprintf(frame, "#%s,%d,%d*%c%c\r\n", mySong.title, mySong.tpm, mySong.nTicks,hex_string[0],hex_string[1]);
}

void createTickFrame(s_tick myTick, char* frame){
    /**
        * Crée une trame de tick au format spécifié à partir des données d'un tick.
        * @param myTick La structure contenant les données du tick.
        * @param frame Le tableau de caractères où stocker la trame créée.
    */
    char * contenu = (char*)malloc(50 * sizeof(char));
    sprintf(contenu, "%02d,%02d,%02d,%02d,%02d,%02d",MODE,myTick.accent,myTick.note[0],myTick.note[1],myTick.note[2],myTick.note[3]);

    int resultat_xor = contenu[0];
    int i=1;
    while(contenu[i]!='\0'){
        resultat_xor ^= contenu[i];
        i++;
    }
    free(contenu);
    char hex_string[2];
    if(resultat_xor<16){
        sprintf(hex_string, "%x", resultat_xor);
        hex_string[1]=hex_string[0];
        hex_string[0]=48;
    }
    else sprintf(hex_string, "%x", resultat_xor);

    sprintf(frame, "#%d,%d,%02d,%02d,%02d,%02d*%c%c\r\n\0",MODE,myTick.accent,myTick.note[0],myTick.note[1],myTick.note[2],myTick.note[3],hex_string[0],hex_string[1]);
}
