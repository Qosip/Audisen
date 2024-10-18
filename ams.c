
#include "define.h"
#include "ams.h"




s_song readAMS(char* fileName){
    /**
        * Lit les données d'une chanson à partir d'un fichier au format AMS et les stocke dans une structure de données song.
        * @param fileName Le nom du fichier contenant les données de la chanson.
        * @return Une structure song contenant les données de la chanson lue depuis le fichier.
    */
    
    s_song mySong;
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        // Remplir la structure avec des valeurs par défaut
        mySong.tpm = 0;
        mySong.nTicks = 0;
        strcpy(mySong.title, "");
        int k=0;
        for(int i=0;i<MAX_NUMBER_TICKS;i++) {
            mySong.tickTab[i].accent=0;
            for (k = 0; k < 4; k++) {
                mySong.tickTab[i].note[k]=0;

            }
        }
        return mySong;
    }
    // Lecture du titre et des informations
    int nombre;
    fgets(mySong.title,MAX_SIZE_TITLE ,file );

    int lentitre = strlen(mySong.title);
    for (int i = 0; i < lentitre; i++) {
        if (mySong.title[i] == '\n' || mySong.title[i] == '\r') {
            mySong.title[i] = '\0'; // Remplace le caractère de retour à la ligne par '\0'// Sort de la boucle dès qu'un caractère de retour à la ligne est trouvé
        }
    }

    char ligne[3];
    fgets(ligne, 4, file);
    nombre=atoi(ligne);
    mySong.tpm = nombre * 2;

    int count = 0;

    rewind(file);
    char zip[MAX_SIZE_LINE];
    while (fgets(zip, MAX_SIZE_LINE, file) != NULL) {
        count++;
    }
    mySong.nTicks=count-4;
    // Lecture des ticks
    char lignetick[MAX_SIZE_LINE];
    int lineNumber = 0;
    rewind(file);
    while (fgets(lignetick, sizeof(lignetick), file)) {
        int compteur=0;
        for (int i = 0; lignetick[i] != '\0'; i++) {

            int position = i / 3; // Pour un tableau de 3 caractères par note
            if (lignetick[i] == '^') {
                mySong.tickTab[lineNumber-4].accent = 1;
                mySong.tickTab[lineNumber-4].note[compteur] = position;
                compteur++;

            } else if (lignetick[i] == 'x') {
                mySong.tickTab[lineNumber-4].note[compteur] = position;
                compteur++;


            }
        }
        lineNumber++;
    }

    fclose(file);
    return mySong;
}



void createAMS(char* txtFileName, char* amsFileName) {
    /**
        * Crée un fichier au format AMS à partir d'un fichier texte contenant les données de la chanson.
        * @param txtFileName Le nom du fichier texte contenant les données de la chanson.
        * @param amsFileName Le nom du fichier AMS à créer.
    */

    char* Notes_Inter [60] = {"B5", "A#5", "A5", "G#5", "G5", "F#5", "F5", "E5", "D#5", "D5", "C#5", "C5","B4", "A#4", "A4", "G#4", "G4", "F#4", "F4", "E4", "D#4", "D4", "C#4", "C4","B3", "A#3", "A3", "G#3", "G3", "F#3", "F3", "E3", "D#3", "D3", "C#3", "C3","B2", "A#2", "A2", "G#2", "G2", "F#2", "F2", "E2", "D#2", "D2", "C#2", "C2","B1", "A#1", "A1", "G#1", "G1", "F#1", "F1", "E1", "D#1", "D1", "C#1", "C1"};
    char Key_notes [5] = {'R', 'B', 'N', 'C', 'D'};
    float Value_notes [5] = {(float)4., (float)2., (float)1., (float)0.5, (float)0.5};

    FILE *fileams = NULL;
    fileams = fopen(amsFileName, "r");
    if (fileams != NULL) {
        fclose(fileams);
        printf("Le fichier %s existe deja -> Il doit etre supprime avant de relancer le programme.\n", amsFileName);
        exit(1);
    }
    fileams = fopen(amsFileName, "w");
    char line[MAX_SIZE_LINE];
    if (fileams == NULL) {
        printf("Erreur lors de l'ouverture du fichier ams.\n");
    }

    FILE *filetxt = NULL;
    if ((filetxt = fopen(txtFileName, "r")) != NULL) {
        //fichier ouvert
    } else {
        return;
    }
    fgets(line, sizeof(line), filetxt);
    fputs(line, fileams);
    fgets(line, sizeof(line), filetxt);
    fputs(line, fileams);
    fgets(line, sizeof(line), filetxt);
    fputs(line, fileams);
    fputs("    ", fileams);
    char val[3] = {0, 0, ' '};
    for (int v = 1; v < 61; v++) {
        val[0] = '0';
        val[1] = '0';
        if (v < 10) {
            val[1] = v + 48;
            fwrite(val, sizeof(char), sizeof(val), fileams);
        } else {
            val[0] = (v / 10) + 48;
            val[1] = (v % 10) + 48;
            fwrite(val, sizeof(char), sizeof(val), fileams);
        }
    }
    fwrite("\n", sizeof(char), 1, fileams);
    long int nbtick = 1;
    memset(line, '\0', sizeof(line));
    int indice_line = 0;
    int prev_indice_line = 0;
    char note[12] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    char temps_note[4] = {'\0', '\0', '\0', '\0'};
    int note_index = 1;
    int note_index2 = 0;

    char prev_note[4][3] = {{'\0', '\0', '\0'}, {'\0', '\0', '\0'}, {'\0', '\0', '\0'}, {'\0', '\0', '\0'}};
    char note_now[12] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    char note_many_times[12] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
    int note_many_times_occur[4] = {0,0,0,0};
    int note_many_times_tick[4] = {0,0,0,0};
    int accent_now[4] = {1,1,1,1};
    int indice_now[4] = {1,1,1,1};
    int note_now_i = 0;

    //prendre le fichier ligne par ligne et extraire les notes,accents et durée chaque note
    while (fgets(line, sizeof(line), filetxt) != NULL) {
        note_index = 1;
        note_index2 = 0;
        // on ecrit le numero du tick au debut de la ligne
        val[0] = (nbtick / 100) + 48;
        val[1] = ((nbtick % 100) / 10) + 48;
        val[2] = (nbtick % 10) + 48;
        fwrite(val, sizeof(char), 3, fileams);
        //////////////////////////////////////////////////
        // remplissage prev_note a partir de note
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                prev_note[i][j] = note_now[j + (i * 3)];
            }
        }
        memset(note_now, '\0', sizeof(note_now));
        memset(note, '\0', sizeof(note));
        memset(temps_note, '\0', sizeof(temps_note));

        while (line[indice_line] != '\r' && line[indice_line] != '\n' && line[indice_line] != '\0') {
            //while (line[indice_line] != ' ' || line[indice_line]!=','){indice_line ++;}

            while ((line[indice_line] >= 'a' && line[indice_line] <= 'z') ||
                   (line[indice_line] >= 'A' && line[indice_line] <= 'Z') || line[indice_line] == '#' ||
                   (line[indice_line] >= '0' && line[indice_line] <= '9')) { indice_line++; }
            note_index = (note_index / 3) * 3;
            for (int i = prev_indice_line; i < indice_line; i++) {
                note[note_index] = line[i];
                note_index++;
            }
            note_index = note_index + 2;
            indice_line++;
            prev_indice_line = indice_line;
            while ((line[indice_line] >= 'a' && line[indice_line] <= 'z') ||
                   (line[indice_line] >= 'A' && line[indice_line] <= 'Z') || line[indice_line] == '#' ||
                   (line[indice_line] >= '0' && line[indice_line] <= '9')) { indice_line++; }
            temps_note[note_index2] = line[indice_line-1];
            note_index2++;
            indice_line = indice_line + 2;
            prev_indice_line = indice_line;
        }
        // ajout des anciennes notes à plus d'un tick dans note_now
        for (int i = 0; i < 4; i++) {
            if (note_many_times[(i)*3] != '\0'){
                for (int j = 0; j < 3; j++) {
                    note_now[i*3+j] = note_many_times[(i)*3+j];
                }
                note_now_i ++;
                note_many_times_tick[(i)]--;
                note_many_times_occur[(i)]++;
                // -- au notes de la ligne précédente avec deja plus de 1 tick et si 0 on memset sa partie de tab
                if (note_many_times_tick[(i)] == 0){
                    note_many_times[(i)*3] = '\0';
                    note_many_times[(i)*3+1] = '\0';
                    note_many_times[(i)*3+2] = '\0';
                    note_many_times_occur[(i)] = 0;
                }
            }
        }
        // comparer temps note pour affecter durée et insérer dans note sur plusieurs ticks
        for (int i = note_now_i; i < 4; i++) {
            for (int j = 0; j < 5 ; j++) {
                if (temps_note[i-note_now_i] == Key_notes[j]) {
                    if ((int)(Value_notes[j]*2) == 1){
                        for (int k = 0; k < 3; k++) {
                            note_now[i*3+k] = note[(i-note_now_i)*3+k];
                        }
                    }
                    else{
                        for (int k = 0; k < 3; k++) {
                            note_now[i*3+k] = note[(i-note_now_i)*3+k];
                        }
                        int pass = 0;
                        for (int l = 0; l < 4; l++) {
                            if (note_many_times[l*3] == '\0' && pass == 0){
                                // stocker les notes durant plus de 1 tick
                                for (int m = 0; m < 3; m++) {
                                    note_many_times[l*3+m] = note_now[(i)*3+m];
                                }
                                note_many_times_tick[l] = (int)(Value_notes[j]*2)-1;
                                pass = 1;
                            }
                        }
                    }
                }
            }
        }
        // voir si note était déjà presente la ligne précédente pour pas la rejouer en accentué
        for (int i = 0; i < note_now_i; i++) {
            for (int j = 0; j < 4; j++) {
                // tab final des notes et accentuation ou non pour ce tick precis
                if (prev_note[j][0] == note_now[i*3] && ((prev_note[j][1] == note_now[i*3+1] && prev_note[j][2] == note_now[i*3+2]) || (prev_note[j][2] == note_now[i*3+1] && prev_note[j][1] == note_now[i*3+2]))){
                    accent_now[i] = 0;
                }
            }
        }
        // avoir l'index des notes
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 60; j++) {
                if ((note_now[i*3] == Notes_Inter[j][0]) && ((note_now[i*3+1] == Notes_Inter[j][1] && note_now[i*3+2] == Notes_Inter[j][2]) || (note_now[i*3+2] == Notes_Inter[j][1] && note_now[i*3+1] == Notes_Inter[j][2]))){
                    indice_now[i] = 60-j;
                }
            }
        }
        //remplissage du tableau
        int already_print = 0;
        for (int j = 1; j < 61; j++) {
            fwrite("|", sizeof(char), 1, fileams);
            for (int i = 0; i < 4; i++) {
                if (indice_now[i] == j && already_print == 0) {
                    if (accent_now[i] != 0)fwrite("^", sizeof(char), 1, fileams);
                    else fwrite("x", sizeof(char), 1, fileams);
                    already_print = 1;
                }
            }
            if (already_print == 0) fwrite(" ", sizeof(char), 1, fileams);
            already_print = 0;
            fwrite(" ", sizeof(char), 1, fileams);
        }
        fwrite("|\n", sizeof(char), 2, fileams);
        nbtick++;
        memset(line, '\0', sizeof(line));
        memset(indice_now, '\0', sizeof(indice_now));
        memset(accent_now, 1, sizeof(accent_now));

        prev_indice_line = 0;
        indice_line = 0;
        note_now_i = 0;
    }
    // Fermeture des fichiers
    fclose(filetxt);
    fclose(fileams);
}


