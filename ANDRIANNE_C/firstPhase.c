#include "firstPhase.h"

void firstPhase(void) {
	double genders[NB_SUBJECTS];
	setGenders(genders);
	char activites[][PATH_SIZE] = { "dws_1", "dws_2", "dws_11", "jog_9", "jog_16", "sit_5", "sit_13", "std_6", "std_14", "ups_3", "ups_4","ups_12", "wlk_7", "wlk_8", "wlk_15" };
	char subjects[][PATH_SIZE] = { "/sub_1", "/sub_2", "/sub_3", "/sub_4", "/sub_5", "/sub_6", "/sub_7", "/sub_8", "/sub_9", "/sub_10", "/sub_11", "/sub_12",
		"/sub_13", "/sub_14", "/sub_15", "/sub_16", "/sub_17","/sub_18", "/sub_19", "/sub_20", "/sub_21", "/sub_22", "/sub_23", "/sub_24" };
	FILE* testSetPtr = NULL;
	FILE* trainSetPtr = NULL;
	FILE* fileReaderPtr = NULL;
	setFiles(trainSetPtr, TRAIN_SET);
	setFiles(testSetPtr, TEST_SET);
	initializeFilePointers(&testSetPtr, &trainSetPtr);
	FILE* createdFilesPointers[] = { testSetPtr, trainSetPtr };


	for (int iAct = 0; iAct < NB_ACTIVITIES; iAct++) {
		int indFile = 0;
		char fileName[FILE_NAME_SIZE];
		for (int iSubject = 0; iSubject < NB_SUBJECTS; iSubject++) {
			strcpy_s(fileName, FILE_NAME_SIZE, ROOT_FOLDER);
			strcat_s(fileName, FILE_NAME_SIZE, activites[iAct]);
			strcat_s(fileName, FILE_NAME_SIZE, subjects[iSubject]);
			strcat_s(fileName, FILE_NAME_SIZE, ".csv");
			printf("Working on file : %s\n", fileName);
			updateFiles(fileReaderPtr, testSetPtr, trainSetPtr, fileName, activites[iAct], iSubject, iAct, genders);
		}
	}


	closeFiles(createdFilesPointers, sizeof(createdFilesPointers) / sizeof(createdFilesPointers[0]));
	puts("Phase 1 is Over");
}

void initializeFilePointers(FILE** testSetPtr, FILE** trainSetPtr) {
	fopen_s(testSetPtr, TEST_SET, "a");
	fopen_s(trainSetPtr, TRAIN_SET, "a");
	if (*testSetPtr == NULL || *trainSetPtr == NULL) {
		printf("Error while opening TRAIN_SET or TEST_SET.csv");
		EXIT;
	}
}

void closeFiles(FILE* filePointers[], int size) {
	for (int i = 0; i < size; i++) {
		fclose(filePointers[i]);
	}
}

void setGenders(double genders[NB_SUBJECTS]) {
	FILE* subjectsInfoPtr;
	fopen_s(&subjectsInfoPtr, "data_subjects_info.csv", "r");
	int iGender = 0;


	if (subjectsInfoPtr == NULL) {
		puts("Error while opening the file !");
		EXIT;
	}

	char row[ROW_SIZE];
	fgets(row, ROW_SIZE, subjectsInfoPtr);
	while (!feof(subjectsInfoPtr)) {
		genders[iGender] = getData(row, 5);
		iGender++;
		fgets(row, ROW_SIZE, subjectsInfoPtr);
	}
	fclose(subjectsInfoPtr);
}

void updateFiles(FILE* fileReaderPtr, FILE* testSetPtr, FILE* trainSetPtr, char fileName[FILE_NAME_SIZE], char path[PATH_SIZE], int iSubject, int iAct, double genders[NB_SUBJECTS]) {
	Record record;
	char row[ROW_SIZE];
	AccelerationVector* pNew = NULL;
	AccelerationVector* pNow = NULL;
	FILE* filePointers[] = { trainSetPtr, testSetPtr };
	bool valid = true;
	int i = 0;

	fopen_s(&fileReaderPtr, fileName, "r");
	if (fileReaderPtr == NULL) {
		printf("Error while opening %s", fileName);
		EXIT;
	}

	fgets(row, ROW_SIZE, fileReaderPtr);
	fgets(row, ROW_SIZE, fileReaderPtr);


	record.activity = getActivity(path);
	record.gender = (int)genders[iSubject];
	record.id = (iSubject + 1) * (iAct + 1);
	record.accVectors = NULL;


	while (valid && i < NB_TENTHS_OF_SECOND) {
		pNew = (AccelerationVector*)malloc(sizeof(AccelerationVector));
		if (pNew == NULL) {
			"No more memory available";
			EXIT;
		}
		double value = sqrt(pow(getData(row, 11), 2) + pow(getData(row, 12), 2) + pow(getData(row, 13), 2));
		pNew->value = value;
		if (value != -1) {
			pNew->next = NULL;
			if (record.accVectors == NULL) {
				record.accVectors = pNew;
			}
			else {
				pNow = record.accVectors;
				while (pNow->next != NULL) {
					pNow = pNow->next;
				}
				pNow->next = pNew;
			}
			fgets(row, ROW_SIZE, fileReaderPtr);
			i++;
		}
		else {
			valid = false;
		}
	}

	int index = rand() % 10 == 1;
	fprintf(filePointers[index], "%s,", record.activity);
	fprintf(filePointers[index], "%d,", record.gender);
	fprintf(filePointers[index], "%d", record.id);
	pNow = record.accVectors;

	while (pNow != NULL) {
		fprintf(filePointers[index], ",%f", pNow->value);
		pNow = pNow->next;
	}
	fprintf(filePointers[index], "\n");
	printf("record written in %s\n", index == 0 ? TEST_SET : TRAIN_SET);

	fclose(fileReaderPtr);
}


char* getActivity(char* activity) {
	char* activities[] = { "dws", "jog", "sit", "std", "ups", "wlk" };

	while (strncmp(activity, *activities, 3) != 0)
		*activities += sizeof(char) * 4;

	return *activities;
}

double getData(char data[], int column) {
	char row[ROW_SIZE];
	strcpy_s(row, ROW_SIZE, data);

	char* nextToken;
	int i = 0;

	for (const char* token = strtok_s(row, ",", &nextToken); token && *token; token = strtok_s(NULL, ",", &nextToken)) {

		i++;

		if (i == column) {
			if (i == 1) {
				if (strcmp("dws", token) == 0) return 1;
				if (strcmp("jog", token) == 0) return 2;
				if (strcmp("sit", token) == 0) return 3;
				if (strcmp("std", token) == 0) return 4;
				if (strcmp("ups", token) == 0) return 5;
				return 6;
			}
			return atof(token);
		}
	}
	return NAN;
}

void setFiles(FILE* filePtr, char* name) {
	fopen_s(&filePtr, name, "w");
	if (filePtr == NULL) {
		printf("Error while opening the file");
		EXIT;
	}
	fprintf(filePtr, "ACTIVITY, GENDER, ID");
	for (int i = 0; i < NB_TENTHS_OF_SECOND; i++) {
		fprintf(filePtr, ",Var%d", i + 1);
	}
	fprintf(filePtr, "\n");
	fclose(filePtr);

}