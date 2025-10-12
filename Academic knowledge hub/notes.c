#include <stdio.h>
#include <string.h>
#include "notes.h"

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
    
    printf("\n✅ Note uploaded successfully!\n");
    printf("[Placeholder] Title: %s, Subject: %s, Faculty: %s\n", title, subject, faculty);
}

void viewAllNotes() {
    printf("\n--- All Notes ---\n");
    printf("[Placeholder] Displaying all notes...\n");
}

void searchNoteBySubject() {
    char subject[50];
    printf("\nEnter subject to search notes: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0; 

    printf("\n[Placeholder] Searching notes for subject: %s\n", subject);
}

void searchNoteByTitle() {
    char title[100];
    printf("\nEnter note title to search: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0; 

    printf("\n[Placeholder] Searching notes with title: %s\n", title);
}