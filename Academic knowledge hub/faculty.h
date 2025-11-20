// ============================================
// FILE: faculty.h
// Faculty Management Module Header
// ============================================

#ifndef FACULTY_H
#define FACULTY_H

#include "structures.h"

typedef struct Faculty {
    char name[MAX_FACULTY_NAME];
    char facultyId[20];
    char department[MAX_DEPT];
    char subject[MAX_SUBJECT];
    char email[50];
    struct Faculty* next;
} Faculty;

// Console functions (original)
void addFaculty();
void removeFaculty();
void viewAllFaculty();
void searchFacultyByDept();
void searchFacultyBySubject();
void searchFacultyById();
void initializeFaculty();
void cleanupFaculty();

// Web API helpers
void addFacultyFromData(const char* name,
                        const char* facultyId,
                        const char* department,
                        const char* subject,
                        const char* email);

int  removeFacultyByIdSimple(const char* facultyId);
void printAllFacultyAsJson();

void loadFaculty();
void saveAllFaculty();

#endif // FACULTY_H
