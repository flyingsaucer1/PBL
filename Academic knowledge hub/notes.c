#include <stdio.h>
#include <string.h>
#include "notes.h"

// ====================== Notes Functions ======================

// Display all notes
void viewAllNotes() {
    printf("\n--- All Notes ---\n");
    printf("[Placeholder] Displaying all notes...\n");
}

// Search notes by subject
void searchNoteBySubject() {
    char subject[50];
    printf("\nEnter subject to search notes: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0; // Remove newline

    printf("\n[Placeholder] Searching notes for subject: %s\n", subject);
}

// Search notes by title
void searchNoteByTitle() {
    char title[100];
    printf("\nEnter note title to search: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline

    printf("\n[Placeholder] Searching notes with title: %s\n", title);
}
