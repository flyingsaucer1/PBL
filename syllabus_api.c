// ============================================
// FILE: syllabus_api.c
// CLI wrapper for Syllabus module (Node backend)
// Commands:
//   syllabus_api add              -> stdin: subject|units
//   syllabus_api list             -> JSON list of all syllabus
//   syllabus_api delete           -> stdin: subject
//   syllabus_api addBook          -> stdin: subject|title|author
//   syllabus_api get              -> stdin: subject   (single syllabus)
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syllabus.h"

extern SyllabusNode* syllabusListHead;

// Convert syllabus list to JSON
void printAllSyllabusAsJson() {
    printf("[");
    SyllabusNode* s = syllabusListHead;
    int first = 1;

    while (s) {
        if (!first) printf(",");
        first = 0;

        printf("{\"subject\":\"%s\",\"units\":\"%s\",\"books\":[", s->subject, s->units);

        BookNode* b = s->books;
        int firstBook = 1;

        while (b) {
            if (!firstBook) printf(",");
            firstBook = 0;

            printf("{\"title\":\"%s\",\"author\":\"%s\"}", b->title, b->author);
            b = b->next;
        }

        printf("]}");
        s = s->next;
    }

    printf("]");
}

// JSON for a single subject
void printSingleSyllabusAsJson(SyllabusNode* s) {
    if (!s) {
        printf("null");
        return;
    }

    printf("{\"subject\":\"%s\",\"units\":\"%s\",\"books\":[", s->subject, s->units);

    BookNode* b = s->books;
    int first = 1;
    while (b) {
        if (!first) printf(",");
        first = 0;
        printf("{\"title\":\"%s\",\"author\":\"%s\"}", b->title, b->author);
        b = b->next;
    }

    printf("]}");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: syllabus_api <add|list|delete|addBook|get>\n");
        return 1;
    }

    initializeSyllabus();

    /* ============================
       ADD SYLLABUS (subject|units)
       ============================ */
    if (strcmp(argv[1], "add") == 0) {
        char line[700];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("{\"success\":false,\"message\":\"No input received\"}");
            cleanupSyllabus();
            return 0;
        }

        char* subject = strtok(line, "|");
        char* units   = strtok(NULL, "|");

        if (!subject || !units) {
            printf("{\"success\":false,\"message\":\"Invalid input format\"}");
            cleanupSyllabus();
            return 0;
        }

        subject[strcspn(subject, "\r\n")] = 0;
        units[strcspn(units, "\r\n")] = 0;

        // Add new syllabus node manually
        SyllabusNode* newS = (SyllabusNode*)malloc(sizeof(SyllabusNode));
        strcpy(newS->subject, subject);
        strcpy(newS->units, units);
        newS->books = NULL;
        newS->next = NULL;

        if (!syllabusListHead) {
            syllabusListHead = newS;
        } else {
            SyllabusNode* t = syllabusListHead;
            while (t->next) t = t->next;
            t->next = newS;
        }

        saveAllSyllabus();

        printf("{\"success\":true,\"message\":\"Syllabus added\"}");
    }

    /* ============================
        LIST ALL SYLLABUS (JSON)
       ============================ */
    else if (strcmp(argv[1], "list") == 0) {
        printAllSyllabusAsJson();
    }

    /* ============================
         DELETE SYLLABUS
         stdin: subject
       ============================ */
    else if (strcmp(argv[1], "delete") == 0) {
        char line[200];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("{\"success\":false,\"message\":\"No subject received\"}");
            cleanupSyllabus();
            return 0;
        }

        char* subject = strtok(line, "\n");
        subject[strcspn(subject, "\r\n")] = 0;

        SyllabusNode* temp = syllabusListHead;
        SyllabusNode* prev = NULL;

        while (temp) {
            if (strcasecmp(temp->subject, subject) == 0) {
                if (prev == NULL) {
                    syllabusListHead = temp->next;
                } else {
                    prev->next = temp->next;
                }

                // Free books
                BookNode* b = temp->books;
                while (b) {
                    BookNode* toFree = b;
                    b = b->next;
                    free(toFree);
                }

                free(temp);
                saveAllSyllabus();
                saveAllBooks();
                printf("{\"success\":true,\"message\":\"Syllabus deleted\"}");
                cleanupSyllabus();
                return 0;
            }

            prev = temp;
            temp = temp->next;
        }

        printf("{\"success\":false,\"message\":\"Subject not found\"}");
    }

    /* ============================
       ADD BOOK (subject|title|author)
       ============================ */
    else if (strcmp(argv[1], "addBook") == 0) {
        char line[400];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("{\"success\":false,\"message\":\"No input received\"}");
            cleanupSyllabus();
            return 0;
        }

        char* subject = strtok(line, "|");
        char* title   = strtok(NULL, "|");
        char* author  = strtok(NULL, "|");

        if (!subject || !title || !author) {
            printf("{\"success\":false,\"message\":\"Invalid book format\"}");
            cleanupSyllabus();
            return 0;
        }

        subject[strcspn(subject, "\r\n")] = 0;
        title[strcspn(title, "\r\n")] = 0;
        author[strcspn(author, "\r\n")] = 0;

        SyllabusNode* s = syllabusListHead;
        while (s) {
            if (strcasecmp(s->subject, subject) == 0) {
                BookNode* b = (BookNode*)malloc(sizeof(BookNode));
                strcpy(b->title, title);
                strcpy(b->author, author);
                b->next = NULL;

                if (!s->books) {
                    s->books = b;
                } else {
                    BookNode* t = s->books;
                    while (t->next) t = t->next;
                    t->next = b;
                }

                saveAllBooks();

                printf("{\"success\":true,\"message\":\"Book added\"}");
                cleanupSyllabus();
                return 0;
            }
            s = s->next;
        }

        printf("{\"success\":false,\"message\":\"Subject not found\"}");
    }

    /* ============================
          GET SINGLE SUBJECT
       ============================ */
    else if (strcmp(argv[1], "get") == 0) {
        char line[200];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("null");
            cleanupSyllabus();
            return 0;
        }

        char* subject = strtok(line, "\n");
        subject[strcspn(subject, "\r\n")] = 0;

        SyllabusNode* s = syllabusListHead;
        while (s) {
            if (strcasecmp(s->subject, subject) == 0) {
                printSingleSyllabusAsJson(s);
                cleanupSyllabus();
                return 0;
            }
            s = s->next;
        }

        printf("null");
    }

    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
    }

    cleanupSyllabus();
    return 0;
}
