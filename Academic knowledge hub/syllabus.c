
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYLLABUS_FILE "syllabus.txt"
#define BOOKS_FILE "books.txt"

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
void addBookToSyllabus();  
void viewAllSyllabus();    
SyllabusNode* syllabusListHead = NULL;

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


void loadSyllabus() {
    FILE* fp = fopen(SYLLABUS_FILE, "r");
    if (!fp) {
        return;
    }

    char line[700];
    while (fgets(line, sizeof(line), fp)) {
        SyllabusNode* newSyl = (SyllabusNode*)malloc(sizeof(SyllabusNode));
        if (!newSyl) continue;

        char* token;
        char lineCopy[700];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(newSyl->subject, token, 99);
        
        token = strtok(NULL, "|");
        if (token) {
            strncpy(newSyl->units, token, 499);
            newSyl->units[strcspn(newSyl->units, "\n")] = 0;
        }

        newSyl->books = NULL;
        newSyl->next = NULL;

        if (!syllabusListHead) {
            syllabusListHead = newSyl;
        } else {
            SyllabusNode* temp = syllabusListHead;
            while (temp->next) temp = temp->next;
            temp->next = newSyl;
        }
    }
    fclose(fp);

    fp = fopen(BOOKS_FILE, "r");
    if (!fp) return;

    while (fgets(line, sizeof(line), fp)) {
        char subject[100], title[100], author[100];
        char* token;
        char lineCopy[700];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(subject, token, 99);
        
        token = strtok(NULL, "|");
        if (token) strncpy(title, token, 99);
        
        token = strtok(NULL, "|");
        if (token) {
            strncpy(author, token, 99);
            author[strcspn(author, "\n")] = 0;
        }

       
        SyllabusNode* s = syllabusListHead;
        while (s) {
            if (strcmp(s->subject, subject) == 0) {
                BookNode* newBook = (BookNode*)malloc(sizeof(BookNode));
                if (newBook) {
                    strcpy(newBook->title, title);
                    strcpy(newBook->author, author);
                    newBook->next = NULL;

                    if (!s->books) {
                        s->books = newBook;
                    } else {
                        BookNode* b = s->books;
                        while (b->next) b = b->next;
                        b->next = newBook;
                    }
                }
                break;
            }
            s = s->next;
        }
    }
    fclose(fp);
    printf(" Loaded syllabus data.\n");
}


void saveAllSyllabus() {
    FILE* fp = fopen(SYLLABUS_FILE, "w");
    if (!fp) {
        printf(" Error: Cannot save syllabus!\n");
        return;
    }

    SyllabusNode* temp = syllabusListHead;
    while (temp) {
        fprintf(fp, "%s|%s\n", temp->subject, temp->units);
        temp = temp->next;
    }
    fflush(fp);
    fclose(fp);
}


void saveAllBooks() {
    FILE* fp = fopen(BOOKS_FILE, "w");
    if (!fp) {
        printf(" Error: Cannot save books!\n");
        return;
    }

    SyllabusNode* s = syllabusListHead;
    while (s) {
        BookNode* b = s->books;
        while (b) {
            fprintf(fp, "%s|%s|%s\n", s->subject, b->title, b->author);
            b = b->next;
        }
        s = s->next;
    }
    fflush(fp);
    fclose(fp);
}

void addSyllabus() {
    SyllabusNode* newSyllabus = (SyllabusNode*)malloc(sizeof(SyllabusNode));
    if (!newSyllabus) {
        printf("\n Memory allocation failed!\n");
        return;
    }

    printf("\n========================================\n");
    printf("      ADD NEW SYLLABUS\n");
    printf("========================================\n");

    printf("Enter Subject: ");
    fgets(newSyllabus->subject, sizeof(newSyllabus->subject), stdin);
    newSyllabus->subject[strcspn(newSyllabus->subject, "\n")] = 0;

    printf("Enter Units/Topics: ");
    fgets(newSyllabus->units, sizeof(newSyllabus->units), stdin);
    newSyllabus->units[strcspn(newSyllabus->units, "\n")] = 0;

    newSyllabus->books = NULL;
    newSyllabus->next = NULL;

    if (!syllabusListHead) {
        syllabusListHead = newSyllabus;
    } else {
        SyllabusNode* temp = syllabusListHead;
        while (temp->next) temp = temp->next;
        temp->next = newSyllabus;
    }

    saveAllSyllabus();
    printf("\n Syllabus added successfully!\n");

    char choice;
    printf("Add reference books? (y/n): ");
    scanf(" %c", &choice);
    int c; 
    while ((c = getchar()) != '\n' && c != EOF);

    if (choice == 'y' || choice == 'Y') addBookToSyllabus();
}

void addBookToSyllabus() {
    if (!syllabusListHead) {
        printf("\n No syllabus available!\n");
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
                    printf("\n Memory allocation failed!\n");
                    return;
                }

                printf("Enter Book Title: ");
                fgets(newBook->title, sizeof(newBook->title), stdin);
                newBook->title[strcspn(newBook->title, "\n")] = 0;

                printf("Enter Book Author: ");
                fgets(newBook->author, sizeof(newBook->author), stdin);
                newBook->author[strcspn(newBook->author, "\n")] = 0;

                newBook->next = NULL;

                if (!temp->books) {
                    temp->books = newBook;
                } else {
                    BookNode* b = temp->books;
                    while (b->next) b = b->next;
                    b->next = newBook;
                }

                saveAllBooks();
                printf("\n Book added successfully!\n");

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

    printf("\n Subject not found!\n");
}

void viewAllSyllabus() {
    if (!syllabusListHead) {
        printf("\n📚 No syllabus available!\n");
        return;
    }

    printf("\n========================================\n");
    printf("      ALL SYLLABUS\n");
    printf("========================================\n");

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
        printf("   📖 Books: %d\n", bookCount);
        printf("----------------------------------------\n");

        temp = temp->next;
        count++;
    }
}

void searchSyllabusBySubject() {
    if (!syllabusListHead) {
        printf("\n📚 No syllabus available!\n");
        return;
    }

    char subject[100];
    printf("\nEnter subject to search: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;

    SyllabusNode* temp = syllabusListHead;
    while (temp) {
        if (compareIgnoreCase(temp->subject, subject)) {
            printf("\n========================================\n");
            printf("Subject: %s\n", temp->subject);
            printf("Units: %s\n", temp->units);
            printf("========================================\n");

            int bookCount = 0;
            BookNode* b = temp->books;
            if (!b) {
                printf("\n📖 No books added yet.\n");
            } else {
                printf("\n📖 REFERENCE BOOKS:\n");
                while (b) {
                    bookCount++;
                    printf("%d. %s by %s\n", bookCount, b->title, b->author);
                    b = b->next;
                }
            }
            return;
        }
        temp = temp->next;
    }

    printf("\n❌ Subject not found!\n");
}

void viewBooksForSubject() {
    if (!syllabusListHead) {
        printf("\n📚 No syllabus available!\n");
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
                printf("\n📖 No books added for this subject.\n");
                return;
            }

            int count = 1;
            printf("\n========================================\n");
            printf("📖 Books for %s:\n", temp->subject);
            printf("========================================\n");
            while (b) {
                printf("%d. %s\n", count, b->title);
                printf("   Author: %s\n", b->author);
                printf("----------------------------------------\n");
                b = b->next;
                count++;
            }
            return;
        }
        temp = temp->next;
    }

    printf("\n❌ Subject not found!\n");
}

void initializeSyllabus() {
    loadSyllabus();
}

void cleanupSyllabus() {
    SyllabusNode* s = syllabusListHead;
    while (s) {
        BookNode* b = s->books;
        while (b) {
            BookNode* toFree = b;
            b = b->next;
            free(toFree);
        }
        SyllabusNode* toFree = s;
        s = s->next;
        free(toFree);
    }
    syllabusListHead = NULL;

}
