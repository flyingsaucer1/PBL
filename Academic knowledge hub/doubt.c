#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubt.h"

#define DOUBTS_FILE "doubts.txt"

Doubt* doubtQueueFront = NULL;
Doubt* doubtQueueRear = NULL;

void loadDoubts() {
    FILE* fp = fopen(DOUBTS_FILE, "r");
    if (!fp) {
        return;
    }

    char line[800];
    while (fgets(line, sizeof(line), fp)) {
        Doubt* newDoubt = (Doubt*)malloc(sizeof(Doubt));
        if (!newDoubt) continue;

        char* token;
        char lineCopy[800];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(newDoubt->studentName, token, 49);
        newDoubt->studentName[49] = '\0';
        
        token = strtok(NULL, "|");
        if (token) strncpy(newDoubt->subject, token, MAX_SUBJECT - 1);
        newDoubt->subject[MAX_SUBJECT - 1] = '\0';
        
        token = strtok(NULL, "|");
        if (token) strncpy(newDoubt->doubtText, token, MAX_DOUBT - 1);
        newDoubt->doubtText[MAX_DOUBT - 1] = '\0';
        
        token = strtok(NULL, "|");
        if (token) strncpy(newDoubt->status, token, 19);
        newDoubt->status[19] = '\0';
        
        token = strtok(NULL, "|");
        if (token) strncpy(newDoubt->solvedBy, token, 49);
        newDoubt->solvedBy[49] = '\0';
        
        token = strtok(NULL, "|");
        if (token) {
            strncpy(newDoubt->solution, token, MAX_DOUBT - 1);
            newDoubt->solution[strcspn(newDoubt->solution, "\n")] = 0;
        }

        newDoubt->next = NULL;

        if (doubtQueueRear == NULL) {
            doubtQueueFront = doubtQueueRear = newDoubt;
        } else {
            doubtQueueRear->next = newDoubt;
            doubtQueueRear = newDoubt;
        }
    }

    fclose(fp);
    printf(" Loaded doubts from file.\n");
}

void saveAllDoubts() {
    FILE* fp = fopen(DOUBTS_FILE, "w");
    if (!fp) {
        printf(" Error: Cannot save doubts!\n");
        return;
    }

    Doubt* temp = doubtQueueFront;
    while (temp) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                temp->studentName, temp->subject, temp->doubtText,
                temp->status, temp->solvedBy, temp->solution);
        temp = temp->next;
    }

    fflush(fp);
    fclose(fp);
}

void postDoubt(char* studentName) {
    Doubt* newDoubt = (Doubt*)malloc(sizeof(Doubt));
    if(!newDoubt) {
        printf("\n Memory allocation failed!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      POST A DOUBT\n");
    printf("========================================\n");
    
    strcpy(newDoubt->studentName, studentName);
    
    printf("Enter Subject: ");
    fgets(newDoubt->subject, MAX_SUBJECT, stdin);
    newDoubt->subject[strcspn(newDoubt->subject, "\n")] = 0;
    
    printf("Enter Your Doubt: ");
    fgets(newDoubt->doubtText, MAX_DOUBT, stdin);
    newDoubt->doubtText[strcspn(newDoubt->doubtText, "\n")] = 0;
    
    strcpy(newDoubt->status, "pending");
    strcpy(newDoubt->solvedBy, "N/A");
    strcpy(newDoubt->solution, "N/A");
    newDoubt->next = NULL;
    
    if(doubtQueueRear == NULL) {
        doubtQueueFront = doubtQueueRear = newDoubt;
    } else {
        doubtQueueRear->next = newDoubt;
        doubtQueueRear = newDoubt;
    }
    
    saveAllDoubts();
    printf("\n✅ Doubt posted successfully!\n");
}

void viewPendingDoubts() {
    if(doubtQueueFront == NULL) {
        printf("\n📝 No doubts in queue!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      PENDING DOUBTS\n");
    printf("========================================\n");
    
    Doubt* temp = doubtQueueFront;
    int count = 1;
    int foundPending = 0;
    
    while(temp != NULL) {
        if(strcmp(temp->status, "pending") == 0) {
            foundPending = 1;
            printf("\n%d. Student: %s\n", count, temp->studentName);
            printf("   Subject: %s\n", temp->subject);
            printf("   Doubt: %s\n", temp->doubtText);
            printf("----------------------------------------\n");
            count++;
        }
        temp = temp->next;
    }
    
    if(!foundPending) {
        printf("\n✅ No pending doubts!\n");
    }
}

void solveDoubt(char* facultyName) {
    if(doubtQueueFront == NULL) {
        printf("\n📝 No doubts available!\n");
        return;
    }
    
    viewPendingDoubts();
    
    char studentName[50];
    printf("\nEnter student name whose doubt to solve: ");
    fgets(studentName, 50, stdin);
    studentName[strcspn(studentName, "\n")] = 0;
    
    Doubt* temp = doubtQueueFront;
    
    while(temp != NULL) {
        if(strcmp(temp->studentName, studentName) == 0 && 
           strcmp(temp->status, "pending") == 0) {
            printf("\n📚 Doubt: %s\n", temp->doubtText);
            printf("Enter solution: ");
            fgets(temp->solution, MAX_DOUBT, stdin);
            temp->solution[strcspn(temp->solution, "\n")] = 0;
            
            strcpy(temp->status, "solved");
            strcpy(temp->solvedBy, facultyName);
            
            saveAllDoubts();
            printf("\n Doubt solved!\n");
            return;
        }
        temp = temp->next;
    }
    
    printf("\n No pending doubt found for that student!\n");
}

void viewSolvedDoubts() {
    if(doubtQueueFront == NULL) {
        printf("\n No doubts available!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      SOLVED DOUBTS\n");
    printf("========================================\n");
    
    Doubt* temp = doubtQueueFront;
    int count = 1;
    int foundSolved = 0;
    
    while(temp != NULL) {
        if(strcmp(temp->status, "solved") == 0) {
            foundSolved = 1;
            printf("\n%d. Student: %s\n", count, temp->studentName);
            printf("   Subject: %s\n", temp->subject);
            printf("   Doubt: %s\n", temp->doubtText);
            printf("   Solution: %s\n", temp->solution);
            printf("   Solved by: %s\n", temp->solvedBy);
            printf("----------------------------------------\n");
            count++;
        }
        temp = temp->next;
    }
    
    if(!foundSolved) {
        printf("\n No solved doubts yet!\n");
    }
}

void viewMyDoubts(char* username) {
    if(doubtQueueFront == NULL) {
        printf("\n You haven't posted any doubts!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      MY DOUBTS\n");
    printf("========================================\n");
    
    Doubt* temp = doubtQueueFront;
    int count = 1;
    int found = 0;
    
    while(temp != NULL) {
        if(strcmp(temp->studentName, username) == 0) {
            found = 1;
            printf("\n%d. Subject: %s\n", count, temp->subject);
            printf("   Doubt: %s\n", temp->doubtText);
            printf("   Status: %s\n", temp->status);
            if(strcmp(temp->status, "solved") == 0) {
                printf("   Solution: %s\n", temp->solution);
                printf("   Solved by: %s\n", temp->solvedBy);
            }
            printf("----------------------------------------\n");
            count++;
        }
        temp = temp->next;
    }
    
    if(!found) {
        printf("\n You haven't posted any doubts!\n");
    }
}

void initializeDoubts() {
    loadDoubts();
}

void cleanupDoubts() {
    Doubt* temp = doubtQueueFront;
    while (temp) {
        Doubt* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    doubtQueueFront = doubtQueueRear = NULL;
}