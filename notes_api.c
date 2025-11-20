// ============================================
// FILE: notes_api.c
// Small wrapper to expose notes as a CLI "API"
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notes.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: notes_api <command>\n");
        return 1;
    }

    // Always load existing notes first
    initializeNotes();

    if (strcmp(argv[1], "add") == 0) {
        // Read one line from stdin: title|subject|content|faculty
        char line[900];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupNotes();
            return 1;
        }

        char* title   = strtok(line, "|");
        char* subject = strtok(NULL, "|");
        char* content = strtok(NULL, "|");
        char* faculty = strtok(NULL, "|");

        if (!title || !subject || !content || !faculty) {
            fprintf(stderr, "Invalid input format.\n");
            cleanupNotes();
            return 1;
        }

        // Remove newline from faculty
        faculty[strcspn(faculty, "\n")] = 0;

        addNoteFromData(title, subject, content, faculty);

        // Respond with OK
        printf("OK\n");
    }
    else if (strcmp(argv[1], "list") == 0) {
        // Print JSON array
        printAllNotesAsJson();
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        cleanupNotes();
        return 1;
    }

    cleanupNotes();
    return 0;
}