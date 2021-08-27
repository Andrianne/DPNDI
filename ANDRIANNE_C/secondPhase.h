#ifndef secondPhase_H
#define secondPhase_H
#include "firstPhase.h"
void setModelFiles(FILE* modelPtrs[], char* filesNames[]);
void setModelFile(FILE* modelPtr, char* fileName);
void setMeans(Mean* meansArray[]);
Mean* setMean();
void updateMeans(Mean* means, char* row);
void writeIntoFiles(FILE* modelsPtrs[], int iActivity, Mean* means[]);
void writeIntoFile(FILE* modelPtr, int iActivity, Mean* mean);
void secondPhase(void);
#endif 