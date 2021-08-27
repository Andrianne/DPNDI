#ifndef firstPhase_H
#define firstPhase_H
#include "libraries.h"
char* getActivity(char* path);
double getData(char data[], int column);
void setFiles(FILE* filePointer, char* name);
void updateFiles(FILE* fileReaderPtr, FILE* testSetPtr, FILE* trainSetPtr, char fileName[FILE_NAME_SIZE], char path[PATH_SIZE], int iSubject, int iAct, double genders[NB_SUBJECTS]);
void initializeFilePointers(FILE** testSetPtr, FILE** trainSetPtr);
void closeFiles(FILE* filePointers[], int size);
void setGenders(double genders[NB_SUBJECTS]);
void firstPhase(void);
#endif