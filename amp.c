
#include "define.h"
#include "amp.h"


FILE* initAMP(char* filename){
    /**
        * Cette fonction initialise un pointeur de fichier en ouvrant un fichier spécifié
        * dans le répertoire parent du répertoire courant.
        *
        * @param filename Le nom du fichier à ouvrir.
        * @return Un pointeur de fichier si le fichier est ouvert avec succès, sinon NULL.
    */
    FILE * pf = NULL;
    if((pf=fopen(filename, "r"))!=NULL) return pf;
    else return NULL;
}

void readAMP(FILE* pf, char * song_filename){
    /**
        * Lit une ligne depuis un fichier et formate le nom de la chanson en remplaçant les espaces par des underscores et en convertissant les lettres en minuscules.
        * @param pf Le pointeur de fichier à partir duquel lire.
        * @param song_filename Le nom de la chanson lu depuis le fichier.
    */
    // Lecture d'une ligne depuis le fichier
    char ligne[40] = {'\0'};
    strncpy(song_filename, ligne, 40);
    if (fgets(ligne, sizeof(ligne), pf) != NULL) {
        int precedentspace = 0;
        int indexfin = 40;
        int i = 0;
        for (i = 0; i < 40; i++) {
            if (ligne[i] == '\n') {
                ligne[i] = '\0';
            } else if (ligne[i] == '\r') {
                ligne[i] = '\0';
                indexfin = i;
            } else if (!((ligne[i] >= 'a' && ligne[i] <= 'z') || (ligne[i] >= 'A' && ligne[i] <= 'Z')) && ligne[i] != '_' && ligne[i] != '\0'){
                if (precedentspace) {
                    for (int j = i; j < 40; j++) {
                        ligne[j] = ligne[j + 1];
                    }
                    i = i-1;
                    ligne[40] = '\0';
                } else {
                    ligne[i] = '_';
                }
                precedentspace = 1;
            }
            if (isupper_(ligne[i])) ligne[i] = tolower_(ligne[i]);
            if (isalpha_(ligne[i]) && ligne[i] != '_') precedentspace = 0;
        }
        strncpy(song_filename, ligne, indexfin);
        strcat(song_filename, ".ams");
    }
}
int isupper_(char c) {
    /**
        * Vérifie si le caractère donné est une lettre majuscule.
        * @param c Le caractère à vérifier.
        * @return 1 si le caractère est une lettre majuscule, sinon 0.
    */
    return c >= 'A' && c <= 'Z';
}
char tolower_(char c) {
    /**
        * Convertit un caractère en minuscule s'il est une lettre majuscule.
        * @param c Le caractère à convertir.
        * @return Le caractère converti en minuscule s'il est une lettre majuscule, sinon le caractère d'origine.
    */
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}
int isalpha_(char c) {
    /**
        * Vérifie si le caractère donné est une lettre alphabétique.
        * @param c Le caractère à vérifier.
        * @return 1 si le caractère est une lettre alphabétique (majuscule ou minuscule), sinon 0.
    */
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void closeAMP(FILE* pf){
    /**
        * Ferme le fichier pointé par le pointeur de fichier donné.
        * @param pf Le pointeur de fichier à fermer.
    */
    if (pf != NULL) {
        fclose(pf);
        printf("Fichier fermé avec succès.\n");
    } else {
        printf("Le pointeur de fichier est NULL. Aucune action n'a été effectuée.\n");
    }
}