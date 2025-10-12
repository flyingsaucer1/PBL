#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faculty.h"

Faculty* facultyListHead = NULL;

void addFaculty() {
    Faculty* newFaculty = (Faculty*)malloc(sizeof(Faculty));
    if(!newFaculty) {
        printf("\n Memory allocation failed!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      ADD NEW FACULTY\n");
    printf("========================================\n");
    
    getchar();
    printf("Enter Faculty Name: ");
    fgets(newFaculty->name, MAX_FACULTY_NAME, stdin);
    newFaculty->name[strcspn(newFaculty->name, "\n")] = 0;
    
    printf("Enter Faculty ID: ");
    fgets(newFaculty->facultyId, 20, stdin);
    newFaculty->facultyId[strcspn(newFaculty->facultyId, "\n")] = 0;
    
    printf("Enter Department: ");
    fgets(newFaculty->department, MAX_DEPT, stdin);
    newFaculty->department[strcspn(newFaculty->department, "\n")] = 0;
    
    printf("Enter Subject: ");
    fgets(newFaculty->subject, MAX_SUBJECT, stdin);
    newFaculty->subject[strcspn(newFaculty->subject, "\n")] = 0;
    
    printf("Enter Email: ");
    fgets(newFaculty->email, 50, stdin);
    newFaculty->email[strcspn(newFaculty->email, "\n")] = 0;
    
    newFaculty->next = NULL;
    
    if(facultyListHead == NULL) {
        facultyListHead = newFaculty;
    } else {
        Faculty* temp = facultyListHead;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFaculty;
    }
    
    printf("\n Faculty added successfully!\n");
}

void removeFaculty() {
    if(facultyListHead == NULL) {
        printf("\n  No faculty records available!\n");
        return;
    }
    
    char facultyId[20];
    printf("\nEnter Faculty ID to remove: ");
    getchar();
    fgets(facultyId, 20, stdin);
    facultyId[strcspn(facultyId, "\n")] = 0;
    
    Faculty* temp = facultyListHead;
    Faculty* prev = NULL;
    
    while(temp != NULL) {
        if(strcmp(temp->facultyId, facultyId) == 0) {
            if(prev == NULL) {
                facultyListHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            printf("\n Faculty %s removed successfully!\n", temp->name);
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    
    printf("\n Faculty ID not found!\n");
}

void viewAllFaculty() {
    if(facultyListHead == NULL) {
        printf("\n  No faculty records available!\n");
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
    getchar();
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
        printf("\n  No faculty found in department: %s\n", dept);
    }
}

void searchFacultyBySubject() {
    char subject[MAX_SUBJECT];
    printf("\nEnter Subject: ");
    getchar();
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
        printf("\n  No faculty found for subject: %s\n", subject);
    }
}

void searchFacultyById() {
    char facultyId[20];
    printf("\nEnter Faculty ID: ");
    getchar();
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