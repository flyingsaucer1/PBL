// ============================================
// FILE: faculty_api.c
// CLI wrapper for Faculty module (for Node)
// Commands:
//   faculty_api add      (stdin: name|id|department|subject|email)
//   faculty_api list     (no stdin) -> JSON array of faculty
//   faculty_api remove   (stdin: facultyId) -> JSON {success, message}
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faculty.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: faculty_api <add|list|remove>\n");
        return 1;
    }

    initializeFaculty();

    if (strcmp(argv[1], "add") == 0) {
        char line[400];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupFaculty();
            return 1;
        }

        char *name       = strtok(line, "|");
        char *facultyId  = strtok(NULL, "|");
        char *department = strtok(NULL, "|");
        char *subject    = strtok(NULL, "|");
        char *email      = strtok(NULL, "|");

        if (!name || !facultyId || !department || !subject || !email) {
            printf("{\"success\":false,\"message\":\"Invalid input format for add.\"}\n");
            cleanupFaculty();
            return 0;
        }

        // Strip newline/CR
        name[strcspn(name, "\r\n")] = 0;
        facultyId[strcspn(facultyId, "\r\n")] = 0;
        department[strcspn(department, "\r\n")] = 0;
        subject[strcspn(subject, "\r\n")] = 0;
        email[strcspn(email, "\r\n")] = 0;

        addFacultyFromData(name, facultyId, department, subject, email);

        printf("{\"success\":true,\"message\":\"Faculty added.\"}\n");
    }
    else if (strcmp(argv[1], "list") == 0) {
        printAllFacultyAsJson();
    }
    else if (strcmp(argv[1], "remove") == 0) {
        char facultyId[100];

        // Prefer facultyId from argv[2] if provided
        if (argc >= 3 && argv[2] && argv[2][0] != '\0') {
            strncpy(facultyId, argv[2], sizeof(facultyId) - 1);
            facultyId[sizeof(facultyId) - 1] = '\0';
        } else {
            // Fallback to reading from stdin (for manual tests)
            if (!fgets(facultyId, sizeof(facultyId), stdin)) {
                printf("{\"success\":false,\"message\":\"No input received.\"}\n");
                cleanupFaculty();
                return 0;
            }
            facultyId[strcspn(facultyId, "\r\n")] = 0;
        }

        int ok = removeFacultyByIdSimple(facultyId);

        if (ok) {
            printf("{\"success\":true,\"message\":\"Faculty removed.\"}\n");
        } else {
            printf("{\"success\":false,\"message\":\"Faculty ID not found.\"}\n");
        }
    }

    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        cleanupFaculty();
        return 1;
    }

    cleanupFaculty();
    return 0;
}
