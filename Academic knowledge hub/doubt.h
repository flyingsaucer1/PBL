#ifndef DOUBT_H
#define DOUBT_H

#include "structures.h"

typedef struct Doubt {
    char studentName[50];
    char subject[MAX_SUBJECT];
    char doubtText[MAX_DOUBT];
    char status[20];
    char solvedBy[50];
    char solution[MAX_DOUBT];
    struct Doubt* next;
} Doubt;

// Original console-facing functions
void postDoubt(char* studentName);
void viewPendingDoubts();
void solveDoubt(char* facultyName);
void viewSolvedDoubts();
void viewMyDoubts(char* username);
void initializeDoubts();
void cleanupDoubts();

// New helpers for web API
void addDoubtFromData(const char* studentName,
                      const char* subject,
                      const char* doubtText);

int solveDoubtFromData(const char* studentName,
                       const char* facultyName,
                       const char* solution);

void printAllDoubtsAsJson();
void printPendingDoubtsAsJson();
void printSolvedDoubtsAsJson();
void printDoubtsForStudentAsJson(const char* username);

#endif
