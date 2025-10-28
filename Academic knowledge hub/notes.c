// ============================================
// FILE: notes.c
// Notes Management - Complete Implementation
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notes.h"

#define NOTES_FILE "notes.txt"

typedef struct Note {
    char title[100];
    char subject[50];
    char content[500];
    char faculty[50];
    struct Note* next;
} Note;

Note* notesListHead = NULL;

void loadNotes() {
    FILE* fp = fopen(NOTES_FILE, "r");
    if (!fp) { 
        return;
    }

    char line[700];
    while (fgets(line, sizeof(line), fp)) {
        Note* newNote = (Note*)malloc(sizeof(Note));
        if (!newNote) continue;

        char* token;
        char lineCopy[700];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(newNote->title, token, 99);
        newNote->title[99] = '\0';
        
        token = strtok(NULL, "|");
        if (token) strncpy(newNote->subject, token, 49);
        newNote->subject[49] = '\0';
        
        token = strtok(NULL, "|");
        if (token) strncpy(newNote->content, token, 499);
        newNote->content[499] = '\0';
        
        token = strtok(NULL, "|");
        if (token) {
            strncpy(newNote->faculty, token, 49);
            newNote->faculty[strcspn(newNote->faculty, "\n")] = 0;
        }

        newNote->next = notesListHead;
        notesListHead = newNote;
    }

    fclose(fp);
    printf(" Loaded notes from file.\n");
}

void saveAllNotes() {
    FILE* fp = fopen(NOTES_FILE, "w");
    if (!fp) {
        printf(" Error: Cannot save notes!\n");
        return;
    }

    Note* temp = notesListHead;
    while (temp) {
        fprintf(fp, "%s|%s|%s|%s\n", 
                temp->title, temp->subject, temp->content, temp->faculty);
        temp = temp->next;
    }

    fflush(fp);
    fclose(fp);
}

void uploadNote() {
    Note* newNote = (Note*)malloc(sizeof(Note));
    if (!newNote) {
        printf("\n Memory allocation failed!\n");
        return;
    }

    printf("\n========================================\n");
    printf("      UPLOAD NOTE/LECTURE\n");
    printf("========================================\n");
    
    printf("Enter Title: ");
    fgets(newNote->title, sizeof(newNote->title), stdin);
    newNote->title[strcspn(newNote->title, "\n")] = 0;
    
    printf("Enter Subject: ");
    fgets(newNote->subject, sizeof(newNote->subject), stdin);
    newNote->subject[strcspn(newNote->subject, "\n")] = 0;
    
    printf("Enter Content/Description: ");
    fgets(newNote->content, sizeof(newNote->content), stdin);
    newNote->content[strcspn(newNote->content, "\n")] = 0;
    
    printf("Enter Faculty Name: ");
    fgets(newNote->faculty, sizeof(newNote->faculty), stdin);
    newNote->faculty[strcspn(newNote->faculty, "\n")] = 0;

    newNote->next = notesListHead;
    notesListHead = newNote;

    saveAllNotes();
    
    printf("\n✅ Note uploaded successfully!\n");
}

void viewAllNotes() {
    if (!notesListHead) {
        printf("\n📝 No notes available yet!\n");
        return;
    }

    printf("\n========================================\n");
    printf("      ALL NOTES/LECTURES\n");
    printf("========================================\n");

    Note* temp = notesListHead;
    int count = 1;

    while (temp) {
        printf("\n%d. Title: %s\n", count, temp->title);
        printf("   Subject: %s\n", temp->subject);
        printf("   Faculty: %s\n", temp->faculty);
        printf("   Content: %s\n", temp->content);
        printf("----------------------------------------\n");
        temp = temp->next;
        count++;
    }
}

void searchNoteBySubject() {
    char subject[50];
    printf("\nEnter subject to search notes: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;

    Note* temp = notesListHead;
    int found = 0;
    int count = 1;

    printf("\n========================================\n");
    printf("      NOTES FOR: %s\n", subject);
    printf("========================================\n");

    while (temp) {
        if (strcasecmp(temp->subject, subject) == 0) {
            found = 1;
            printf("\n%d. Title: %s\n", count, temp->title);
            printf("   Faculty: %s\n", temp->faculty);
            printf("   Content: %s\n", temp->content);
            printf("----------------------------------------\n");
            count++;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("\n No notes found for subject: %s\n", subject);
    }
}

void searchNoteByTitle() {
    char title[100];
    printf("\nEnter note title to search: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    Note* temp = notesListHead;
    int found = 0;

    while (temp) {
        if (strcasecmp(temp->title, title) == 0) {
            found = 1;
            printf("\n========================================\n");
            printf("      NOTE DETAILS\n");
            printf("========================================\n");
            printf("Title: %s\n", temp->title);
            printf("Subject: %s\n", temp->subject);
            printf("Faculty: %s\n", temp->faculty);
            printf("Content: %s\n", temp->content);
            printf("========================================\n");
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("\n Note not found: %s\n", title);
    }
}

void initializeNotes() {
    loadNotes();
}

void cleanupNotes() {
    Note* temp = notesListHead;
    while (temp) {
        Note* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    notesListHead = NULL;
}