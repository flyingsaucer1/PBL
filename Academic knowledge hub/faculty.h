#ifndef FACULTY_H
#define FACULTY_H
#include "structures.h"
#define MAX_FACULTY_NAME 50
#define MAX_DEPT 30

typedef struct Faculty {
    char name[MAX_FACULTY_NAME];
    char facultyId[20];
    char department[MAX_DEPT];
    char subject[MAX_SUBJECT];
    char email[50];
    struct Faculty* next;
} Faculty;

extern Faculty* facultyListHead;

void addFaculty();
void removeFaculty();
void viewAllFaculty();
void searchFacultyByDept();
void searchFacultyBySubject();
void searchFacultyById();

#endif
