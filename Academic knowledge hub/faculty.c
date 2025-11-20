// ============================================
// FILE: faculty.c
// Faculty Management - Console + Web API Helpers
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faculty.h"

#define FACULTY_FILE "faculty.txt"

Faculty* facultyListHead = NULL;

// =======================
// Internal load/save
// =======================

void loadFaculty() {
    FILE* fp = fopen(FACULTY_FILE, "r");
    if (!fp) {
        return;
    }

    char line[400];
    while (fgets(line, sizeof(line), fp)) {
        Faculty* newFaculty = (Faculty*)malloc(sizeof(Faculty));
        if (!newFaculty) continue;

        char* token;
        char lineCopy[400];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(newFaculty->name, token, MAX_FACULTY_NAME - 1);
        newFaculty->name[MAX_FACULTY_NAME - 1] = '\0';

        token = strtok(NULL, "|");
        if (token) strncpy(newFaculty->facultyId, token, 19);
        newFaculty->facultyId[19] = '\0';

        token = strtok(NULL, "|");
        if (token) strncpy(newFaculty->department, token, MAX_DEPT - 1);
        newFaculty->department[MAX_DEPT - 1] = '\0';

        token = strtok(NULL, "|");
        if (token) strncpy(newFaculty->subject, token, MAX_SUBJECT - 1);
        newFaculty->subject[MAX_SUBJECT - 1] = '\0';

        token = strtok(NULL, "|");
        if (token) {
            strncpy(newFaculty->email, token, 49);
            newFaculty->email[49] = '\0';
            newFaculty->email[strcspn(newFaculty->email, "\n")] = 0;
        }

        newFaculty->next = NULL;

        if (!facultyListHead) {
            facultyListHead = newFaculty;
        } else {
            Faculty* temp = facultyListHead;
            while (temp->next) temp = temp->next;
            temp->next = newFaculty;
        }
    }

    fclose(fp);
    //printf("✅ Loaded faculty records.\n");
}

void saveAllFaculty() {
    FILE* fp = fopen(FACULTY_FILE, "w");
    if (!fp) {
        fprintf(stderr, " Error: Cannot save faculty!\n");
        return;
    }

    Faculty* temp = facultyListHead;
    while (temp) {
        fprintf(fp, "%s|%s|%s|%s|%s\n",
                temp->name, temp->facultyId, temp->department,
                temp->subject, temp->email);
        temp = temp->next;
    }

    fflush(fp);
    fclose(fp);
}

// ===================================
// New helpers for web API / programmatic calls
// ===================================

void addFacultyFromData(const char* name,
                        const char* facultyId,
                        const char* department,
                        const char* subject,
                        const char* email) {
    Faculty* newFaculty = (Faculty*)malloc(sizeof(Faculty));
    if (!newFaculty) {
        fprintf(stderr, " Memory allocation failed in addFacultyFromData!\n");
        return;
    }

    strncpy(newFaculty->name, name, MAX_FACULTY_NAME - 1);
    newFaculty->name[MAX_FACULTY_NAME - 1] = '\0';

    strncpy(newFaculty->facultyId, facultyId, 19);
    newFaculty->facultyId[19] = '\0';

    strncpy(newFaculty->department, department, MAX_DEPT - 1);
    newFaculty->department[MAX_DEPT - 1] = '\0';

    strncpy(newFaculty->subject, subject, MAX_SUBJECT - 1);
    newFaculty->subject[MAX_SUBJECT - 1] = '\0';

    strncpy(newFaculty->email, email, 49);
    newFaculty->email[49] = '\0';

    newFaculty->next = NULL;

    if (facultyListHead == NULL) {
        facultyListHead = newFaculty;
    } else {
        Faculty* temp = facultyListHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFaculty;
    }

    saveAllFaculty();
}

/**
 * removeFacultyByIdSimple:
 *  returns 1 if removed, 0 if not found
 */
int removeFacultyByIdSimple(const char* facultyId) {
    if (facultyListHead == NULL) {
        return 0;
    }

    Faculty* temp = facultyListHead;
    Faculty* prev = NULL;

    while (temp != NULL) {
        if (strcmp(temp->facultyId, facultyId) == 0) {
            if (prev == NULL) {
                facultyListHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            saveAllFaculty();
            return 1;
        }
        prev = temp;
        temp = temp->next;
    }

    return 0;
}

// JSON printers (simple, no escaping of quotes inside data)
void printAllFacultyAsJson() {
    Faculty* temp = facultyListHead;
    int first = 1;

    printf("[\n");
    while (temp) {
        if (!first) printf(",\n");
        first = 0;

        printf("  {\"name\":\"%s\",\"facultyId\":\"%s\",\"department\":\"%s\","
               "\"subject\":\"%s\",\"email\":\"%s\"}",
               temp->name, temp->facultyId, temp->department,
               temp->subject, temp->email);

        temp = temp->next;
    }
    printf("\n]\n");
}

// =======================
// Original console functions
// =======================

void addFaculty() {
    char name[MAX_FACULTY_NAME];
    char facultyId[20];
    char department[MAX_DEPT];
    char subject[MAX_SUBJECT];
    char email[50];

    printf("\n========================================\n");
    printf("      ADD NEW FACULTY\n");
    printf("========================================\n");

    printf("Enter Faculty Name: ");
    fgets(name, MAX_FACULTY_NAME, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter Faculty ID: ");
    fgets(facultyId, 20, stdin);
    facultyId[strcspn(facultyId, "\n")] = 0;

    printf("Enter Department: ");
    fgets(department, MAX_DEPT, stdin);
    department[strcspn(department, "\n")] = 0;

    printf("Enter Subject: ");
    fgets(subject, MAX_SUBJECT, stdin);
    subject[strcspn(subject, "\n")] = 0;

    printf("Enter Email: ");
    fgets(email, 50, stdin);
    email[strcspn(email, "\n")] = 0;

    addFacultyFromData(name, facultyId, department, subject, email);

    printf("\n Faculty added successfully!\n");
}

void removeFaculty() {
    if(facultyListHead == NULL) {
        printf("\n No faculty records available!\n");
        return;
    }

    char facultyId[20];
    printf("\nEnter Faculty ID to remove: ");
    fgets(facultyId, 20, stdin);
    facultyId[strcspn(facultyId, "\n")] = 0;

    int ok = removeFacultyByIdSimple(facultyId);
    if (ok) {
        printf("\n Faculty removed successfully!\n");
    } else {
        printf("\n Faculty ID not found!\n");
    }
}

void viewAllFaculty() {
    if(facultyListHead == NULL) {
        printf("\n No faculty records available!\n");
        return;
    }

    printf("\n========================================\n");
    printf("      ALL FACULTY MEMBERS\n");
    printf("========================================\n");

    Faculty* temp = facultyListHead;
    int count = 1;

    while(temp != NULL) {
        printf("\n%d. Name: %s\n", count, temp->name);
        printf("   Faculty ID: %s\n", temp->facultyId);
        printf("   Department: %s\n", temp->department);
        printf("   Subject: %s\n", temp->subject);
        printf("   Email: %s\n", temp->email);
        printf("----------------------------------------\n");
        temp = temp->next;
        count++;
    }
}

void searchFacultyByDept() {
    char dept[MAX_DEPT];
    printf("\nEnter Department: ");
    fgets(dept, MAX_DEPT, stdin);
    dept[strcspn(dept, "\n")] = 0;

    Faculty* temp = facultyListHead;
    int found = 0;

    printf("\n========================================\n");
    printf("      SEARCH RESULTS - %s\n", dept);
    printf("========================================\n");

    while(temp != NULL) {
        if(strcasecmp(temp->department, dept) == 0) {
            found = 1;
            printf("\nName: %s\n", temp->name);
            printf("Faculty ID: %s\n", temp->facultyId);
            printf("Subject: %s\n", temp->subject);
            printf("Email: %s\n", temp->email);
            printf("----------------------------------------\n");
        }
        temp = temp->next;
    }

    if(!found) {
        printf("\n No faculty found in department: %s\n", dept);
    }
}

void searchFacultyBySubject() {
    char subject[MAX_SUBJECT];
    printf("\nEnter Subject: ");
    fgets(subject, MAX_SUBJECT, stdin);
    subject[strcspn(subject, "\n")] = 0;

    Faculty* temp = facultyListHead;
    int found = 0;

    printf("\n========================================\n");
    printf("      SEARCH RESULTS - %s\n", subject);
    printf("========================================\n");

    while(temp != NULL) {
        if(strcasecmp(temp->subject, subject) == 0) {
            found = 1;
            printf("\nName: %s\n", temp->name);
            printf("Faculty ID: %s\n", temp->facultyId);
            printf("Department: %s\n", temp->department);
            printf("Email: %s\n", temp->email);
            printf("----------------------------------------\n");
        }
        temp = temp->next;
    }

    if(!found) {
        printf("\n No faculty found for subject: %s\n", subject);
    }
}

void searchFacultyById() {
    char facultyId[20];
    printf("\nEnter Faculty ID: ");
    fgets(facultyId, 20, stdin);
    facultyId[strcspn(facultyId, "\n")] = 0;

    Faculty* temp = facultyListHead;

    while(temp != NULL) {
        if(strcmp(temp->facultyId, facultyId) == 0) {
            printf("\n========================================\n");
            printf("      FACULTY DETAILS\n");
            printf("========================================\n");
            printf("Name: %s\n", temp->name);
            printf("Faculty ID: %s\n", temp->facultyId);
            printf("Department: %s\n", temp->department);
            printf("Subject: %s\n", temp->subject);
            printf("Email: %s\n", temp->email);
            printf("========================================\n");
            return;
        }
        temp = temp->next;
    }

    printf("\n Faculty ID not found!\n");
}

void initializeFaculty() {
    loadFaculty();
}

void cleanupFaculty() {
    Faculty* temp = facultyListHead;
    while (temp) {
        Faculty* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    facultyListHead = NULL;
}
