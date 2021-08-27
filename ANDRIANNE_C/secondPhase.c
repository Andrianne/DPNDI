#include "secondPhase.h"

void secondPhase(void) {
	FILE* modelPtr = NULL;
	FILE* modelWomenPtr = NULL;
	FILE* modelMenPtr = NULL;
	FILE* trainSetPtr = NULL;
	FILE* modelsPtrs[3];
	char* modelsNames[] = { MODEL, MODEL_WOMEN, MODEL_MEN };
	Mean* means = malloc(sizeof(Mean*));
	Mean* womenMeans = malloc(sizeof(Mean*));
	Mean* menMeans = malloc(sizeof(Mean*));
	Mean* meansArray[] = { means, womenMeans, menMeans };

	setModelFiles(modelsPtrs, modelsNames);
	setMeans(meansArray);

	fopen_s(&trainSetPtr, TRAIN_SET, "r");
	fopen_s(&modelPtr, MODEL, "a");
	fopen_s(&modelWomenPtr, MODEL_WOMEN, "a");
	fopen_s(&modelMenPtr, MODEL_MEN, "a");
	modelsPtrs[0] = modelPtr;
	modelsPtrs[1] = modelWomenPtr;
	modelsPtrs[2] = modelMenPtr;

	if (trainSetPtr == NULL || modelPtr == NULL || modelWomenPtr == NULL || modelMenPtr == NULL) {
		printf("Error while one of those files");
		EXIT;
	}
	char row[ROW_SIZE];

	//2 calls becasuse of the first line which contains the titles
	fgets(row, ROW_SIZE, trainSetPtr);
	fgets(row, ROW_SIZE, trainSetPtr);

	int iActivity = 0;
	while (!feof(trainSetPtr) && trainSetPtr != NULL) {
		iActivity = getData(row, 1);
		while (!feof(trainSetPtr) && iActivity == getData(row, 1)) {
			updateMeans(meansArray[0], row);
			updateMeans(getData(row, 2) == 0 ? meansArray[1] : meansArray[2], row);
			fgets(row, ROW_SIZE, trainSetPtr);
		}
		writeIntoFiles(modelsPtrs, iActivity, meansArray);
		printf("Movement %d is finished\n", iActivity);
		setMeans(meansArray);
	}
	closeFiles(modelsPtrs, sizeof(modelsPtrs) / sizeof(modelsPtrs[0]));

	puts("Phase 2 Over");
}

void setModelFiles(FILE* modelPtrs[], char* fileNames[]) {
	for (int i = 0; i < 3; i++) {
		setModelFile(modelPtrs[i], fileNames[i]);
	}
}

void setModelFile(FILE* modelPtr, char* fileName) {
	fopen_s(&modelPtr, fileName, "w");
	if (modelPtr == NULL) {
		puts("Error while opening the file;");
		EXIT;
	}
	fprintf(modelPtr, "MOUVEMENT");
	for (int i = 0; i < NB_TENTHS_OF_SECOND; i++) {
		fprintf(modelPtr, ",Vacc%d", i + 1);
	}
	fprintf(modelPtr, "\n");
	fclose(modelPtr);
}

void setMeans(Mean* meansArray[]) {
	for (int i = 0; i < 3; i++) {
		meansArray[i] = setMean();
	}
}

Mean* setMean() {
	Mean* pMeans = NULL;
	Mean* pFirst = NULL;
	Mean* pSavedMeans = NULL;

	for (int j = 0; j < NB_TENTHS_OF_SECOND; j++) {
		Mean* pNewMean = malloc(sizeof(Mean));
		if (pNewMean == NULL) {
			printf("No more memory available");
			EXIT;
		}
		pNewMean->nbTimes = 0;
		pNewMean->totalTenthsOfSecond = 0;
		pNewMean->next = NULL;
		pMeans = pFirst;
		while (pMeans != NULL) {
			pSavedMeans = pMeans;
			pMeans = pMeans->next;
		}
		if (pFirst == NULL)
			pFirst = pNewMean;
		else {
			pSavedMeans->next = pNewMean;
		}
	}

	return pFirst;
}

void updateMeans(Mean* means, char* row) {
	Mean* pMeans = means;
	int i = 0;
	while (pMeans != NULL) {
		pMeans->totalTenthsOfSecond += getData(row, i + 4);
		pMeans->nbTimes++;
		pMeans = pMeans->next;
		i++;
	}
}

void writeIntoFiles(FILE* modelsPtrs[], int iActivity, Mean* means[]) {
	for (int i = 0; i < 3; i++) {
		writeIntoFile(modelsPtrs[i], iActivity, means[i]);
	}
}

void writeIntoFile(FILE* modelPtr, int iActivity, Mean* mean) {
	fprintf(modelPtr, "%d", iActivity);
	Mean* pMean = mean;
	while (pMean != NULL) {
		fprintf(modelPtr, ",%f", pMean->totalTenthsOfSecond / pMean->nbTimes);
		pMean = pMean->next;
	}
	fprintf(modelPtr, "\n");
}

