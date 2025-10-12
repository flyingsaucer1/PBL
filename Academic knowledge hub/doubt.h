#ifndef DOUBT_H
#define DOUBT_H
#include "structures.h"
#define MAX_DOUBT 200

typedef struct Doubt {
    char studentName[50];
    char subject[MAX_SUBJECT];
    char doubtText[MAX_DOUBT];
    char status[20];      
    char solvedBy[50];
    char solution[MAX_DOUBT];
    struct Doubt* next;
} Doubt;

void postDoubt(char* username);
void solveDoubt(char* facultyName);
void viewMyDoubts(char* username);
void viewPendingDoubts();
void viewSolvedDoubts();

#endif
