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

// =======================
// Internal helpers
// =======================

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
            newNote->faculty[49] = '\0';
            newNote->faculty[strcspn(newNote->faculty, "\n")] = 0;
        }

        newNote->next = notesListHead;
        notesListHead = newNote;
    }

    fclose(fp);
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

// ===================================
// New helper for web API / programmatic calls
// ===================================
void addNoteFromData(const char* title,
                     const char* subject,
                     const char* content,
                     const char* faculty) {
    Note* newNote = (Note*)malloc(sizeof(Note));
    if (!newNote) {
        printf(" Memory allocation failed!\n");
        return;
    }

    strncpy(newNote->title, title, sizeof(newNote->title) - 1);
    newNote->title[sizeof(newNote->title) - 1] = '\0';

    strncpy(newNote->subject, subject, sizeof(newNote->subject) - 1);
    newNote->subject[sizeof(newNote->subject) - 1] = '\0';

    strncpy(newNote->content, content, sizeof(newNote->content) - 1);
    newNote->content[sizeof(newNote->content) - 1] = '\0';

    strncpy(newNote->faculty, faculty, sizeof(newNote->faculty) - 1);
    newNote->faculty[sizeof(newNote->faculty) - 1] = '\0';

    newNote->next = notesListHead;
    notesListHead = newNote;

    saveAllNotes();
}

// ===================================
// New helper to print all notes as JSON
// (used by notes_api.c)
// ===================================
void printAllNotesAsJson() {
    Note* temp = notesListHead;
    printf("[\n");
    int first = 1;
    while (temp) {
        if (!first) {
            printf(",\n");
        }
        first = 0;

        // NOTE: This is a simple JSON printer. Avoid using quotes in your data.
        printf("  {\"title\":\"%s\",\"subject\":\"%s\",\"content\":\"%s\",\"faculty\":\"%s\"}",
               temp->title, temp->subject, temp->content, temp->faculty);

        temp = temp->next;
    }
    printf("\n]\n");
}

// =======================
// Original console functions
// =======================

void uploadNote() {
    char title[100], subject[50], content[500], faculty[50];

    printf("\n========================================\n");
    printf("      UPLOAD NOTE/LECTURE\n");
    printf("========================================\n");
    
    printf("Enter Title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;
    
    printf("Enter Subject: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;
    
    printf("Enter Content/Description: ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    
    printf("Enter Faculty Name: ");
    fgets(faculty, sizeof(faculty), stdin);
    faculty[strcspn(faculty, "\n")] = 0;

    addNoteFromData(title, subject, content, faculty);
    
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
        // strcasecmp may be different on Windows; adjust if needed.
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