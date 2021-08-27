#ifndef CONST_H
#define CONST_H
#define NB_ACTIVITIES 15
#define NB_SUBJECTS 24
#define NB_TENTHS_OF_SECOND 600
#define PATH_SIZE 8
#define FILE_NAME_SIZE 50
#define ROOT_FOLDER "data/"
#define ROW_SIZE 10000
#define TRAIN_SET "trainSet.csv"
#define TEST_SET "testSet.csv"
#define MODEL "FI_MODEL.csv"
#define MODEL_WOMEN "FI_MODEL_WOMEN.csv"
#define MODEL_MEN "FI_MODEL_MEN.csv"
#define	EXIT system("pause"); exit(EXIT_FAILURE);

typedef struct accelerationVector AccelerationVector;
struct accelerationVector {
	double value;
	AccelerationVector* next;
};

typedef struct record Record;
struct record {
	char* activity;
	int gender;
	int id;
	AccelerationVector* accVectors;
};

typedef struct mean Mean;
struct mean {
	int nbTimes;
	double totalTenthsOfSecond;
	Mean* next;
};
#endif