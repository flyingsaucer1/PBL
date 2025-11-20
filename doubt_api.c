// ============================================
// FILE: doubt_api.c
// CLI wrapper for Doubt module (for Node)
// Commands:
//   doubt_api add       (stdin: studentName|subject|doubtText)
//   doubt_api list      (no stdin) -> all doubts JSON
//   doubt_api listPending  -> pending doubts JSON
//   doubt_api listSolved   -> solved doubts JSON
//   doubt_api listMy    (stdin: username)
//   doubt_api solve     (stdin: studentName|facultyName|solution)
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubt.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: doubt_api <add|list|listPending|listSolved|listMy|solve>\n");
        return 1;
    }

    initializeDoubts();

    if (strcmp(argv[1], "add") == 0) {
        char line[800];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupDoubts();
            return 1;
        }

        char *studentName = strtok(line, "|");
        char *subject     = strtok(NULL, "|");
        char *doubtText   = strtok(NULL, "|");

        if (!studentName || !subject || !doubtText) {
            fprintf(stderr, "Invalid input format for add.\n");
            cleanupDoubts();
            return 1;
        }

        studentName[strcspn(studentName, "\r\n")] = 0;
        subject[strcspn(subject, "\r\n")] = 0;
        doubtText[strcspn(doubtText, "\r\n")] = 0;

        addDoubtFromData(studentName, subject, doubtText);

        // Simple OK (Node doesn't parse JSON here)
        printf("OK\n");
    }
    else if (strcmp(argv[1], "list") == 0) {
        printAllDoubtsAsJson();
    }
    else if (strcmp(argv[1], "listPending") == 0) {
        printPendingDoubtsAsJson();
    }
    else if (strcmp(argv[1], "listSolved") == 0) {
        printSolvedDoubtsAsJson();
    }
    else if (strcmp(argv[1], "listMy") == 0) {
        char line[200];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupDoubts();
            return 1;
        }
        char* username = strtok(line, "\n");
        if (!username) {
            fprintf(stderr, "Invalid input format for listMy.\n");
            cleanupDoubts();
            return 1;
        }
        username[strcspn(username, "\r\n")] = 0;

        printDoubtsForStudentAsJson(username);
    }
    else if (strcmp(argv[1], "solve") == 0) {
        char line[800];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupDoubts();
            return 1;
        }

        char *studentName = strtok(line, "|");
        char *facultyName = strtok(NULL, "|");
        char *solution    = strtok(NULL, "|");

        if (!studentName || !facultyName || !solution) {
            fprintf(stderr, "Invalid input format for solve.\n");
            cleanupDoubts();
            return 1;
        }

        studentName[strcspn(studentName, "\r\n")] = 0;
        facultyName[strcspn(facultyName, "\r\n")] = 0;
        solution[strcspn(solution, "\r\n")] = 0;

        int ok = solveDoubtFromData(studentName, facultyName, solution);

        printf("{\"success\":%s,\"message\":\"%s\"}\n",
               ok ? "true" : "false",
               ok ? "Doubt marked as solved." : "No pending doubt found for that student.");
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        cleanupDoubts();
        return 1;
    }

    cleanupDoubts();
    return 0;
}
