#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BookNode {
    char title[100];
    char author[100];
    struct BookNode* next;
} BookNode;

typedef struct SyllabusNode {
    char subject[100];
    char units[500];
    BookNode* books;
    struct SyllabusNode* next;
} SyllabusNode;

SyllabusNode* syllabusListHead = NULL;

void addSyllabus();
void addBookToSyllabus();
void viewAllSyllabus();
void searchSyllabusBySubject();
void viewBooksForSubject();

int compareIgnoreCase(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] && str2[i]) {
        char c1 = str1[i];
        char c2 = str2[i];
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        if (c1 != c2) return 0;
        i++;
    }
    return (str1[i] == str2[i]);
}

void addSyllabus() {
    SyllabusNode* newSyllabus = (SyllabusNode*)malloc(sizeof(SyllabusNode));
    if (!newSyllabus) {
        printf("\nMemory allocation failed!\n");
        return;
    }

    printf("\nEnter Subject: ");
    fgets(newSyllabus->subject, sizeof(newSyllabus->subject), stdin);
    newSyllabus->subject[strcspn(newSyllabus->subject, "\n")] = 0;

    printf("Enter Units/Topics: ");
    fgets(newSyllabus->units, sizeof(newSyllabus->units), stdin);
    newSyllabus->units[strcspn(newSyllabus->units, "\n")] = 0;

    newSyllabus->books = NULL;
    newSyllabus->next = NULL;

    if (!syllabusListHead) syllabusListHead = newSyllabus;
    else {
        SyllabusNode* temp = syllabusListHead;
        while (temp->next) temp = temp->next;
        temp->next = newSyllabus;
    }

    printf("\nSyllabus added successfully!\n");

    char choice;
    printf("Add reference books? (y/n): ");
    scanf(" %c", &choice);
    int c; while ((c = getchar()) != '\n' && c != EOF);

    if (choice == 'y' || choice == 'Y') addBookToSyllabus();
}

void addBookToSyllabus() {
    if (!syllabusListHead) {
        printf("\nNo syllabus available!\n");
        return;
    }

    viewAllSyllabus();

    char subject[100];
    printf("\nEnter subject to add book: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;

    SyllabusNode* temp = syllabusListHead;
    while (temp) {
        if (compareIgnoreCase(temp->subject, subject)) {
            int c;
            do {
                BookNode* newBook = (BookNode*)malloc(sizeof(BookNode));
                if (!newBook) {
                    printf("\nMemory allocation failed!\n");
                    return;
                }

                printf("Enter Book Title: ");
                fgets(newBook->title, sizeof(newBook->title), stdin);
                newBook->title[strcspn(newBook->title, "\n")] = 0;

                printf("Enter Book Author: ");
                fgets(newBook->author, sizeof(newBook->author), stdin);
                newBook->author[strcspn(newBook->author, "\n")] = 0;

                newBook->next = NULL;

                if (!temp->books) temp->books = newBook;
                else {
                    BookNode* b = temp->books;
                    while (b->next) b = b->next;
                    b->next = newBook;
                }

                printf("\nBook added successfully!\n");

                char more;
                printf("Add another book? (y/n): ");
                scanf(" %c", &more);
                while ((c = getchar()) != '\n' && c != EOF);
                if (more != 'y' && more != 'Y') break;

            } while (1);
            return;
        }
        temp = temp->next;
    }

    printf("\nSubject not found!\n");
}

void viewAllSyllabus() {
    if (!syllabusListHead) {
        printf("\nNo syllabus available!\n");
        return;
    }

    SyllabusNode* temp = syllabusListHead;
    int count = 1;

    while (temp) {
        printf("\n%d. Subject: %s\n", count, temp->subject);
        printf("   Units: %s\n", temp->units);

        int bookCount = 0;
        BookNode* b = temp->books;
        while (b) {
            bookCount++;
            b = b->next;
        }
        printf("   Books: %d\n", bookCount);
        printf("----------------------------------------\n");

        temp = temp->next;
        count++;
    }
}

void searchSyllabusBySubject() {
    if (!syllabusListHead) {
        printf("\nNo syllabus available!\n");
        return;
    }

    char subject[100];
    printf("\nEnter subject to search: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;

    SyllabusNode* temp = syllabusListHead;
    while (temp) {
        if (compareIgnoreCase(temp->subject, subject)) {
            printf("\nSubject: %s\nUnits: %s\n", temp->subject, temp->units);

            int bookCount = 0;
            BookNode* b = temp->books;
            while (b) {
                bookCount++;
                printf("Book %d: %s by %s\n", bookCount, b->title, b->author);
                b = b->next;
            }
            return;
        }
        temp = temp->next;
    }

    printf("\nSubject not found!\n");
}

void viewBooksForSubject() {
    if (!syllabusListHead) {
        printf("\nNo syllabus available!\n");
        return;
    }

    char subject[100];
    printf("\nEnter subject to view books: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;

    SyllabusNode* temp = syllabusListHead;
    while (temp) {
        if (compareIgnoreCase(temp->subject, subject)) {
            BookNode* b = temp->books;
            if (!b) {
                printf("\nNo books added for this subject.\n");
                return;
            }

            int count = 1;
            printf("\nBooks for %s:\n", temp->subject);
            while (b) {
                printf("%d. %s by %s\n", count++, b->title, b->author);
                b = b->next;
            }
            return;
        }
        temp = temp->next;
    }

    printf("\nSubject not found!\n");
}
