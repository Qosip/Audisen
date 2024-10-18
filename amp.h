#ifndef AUDISEN2024_AMP_H
#define AUDISEN2024_AMP_H

#include "define.h"


FILE* initAMP(char* filename);

void readAMP(FILE* pf, char * song_filename);

void closeAMP(FILE* pf);
int isupper_(char c);
char tolower_(char c);
int isalpha_(char c);

#endif //AUDISEN2024_AMP_H