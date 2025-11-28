#ifndef SYLLABUS_H
#define SYLLABUS_H
#include "structures.h"

#define MAX_UNITS 500
#define MAX_BOOK_TITLE 100
#define MAX_BOOK_AUTHOR 100

typedef struct BookNode {
    char title[MAX_BOOK_TITLE];
    char author[MAX_BOOK_AUTHOR];
    struct BookNode* next;
} BookNode;

typedef struct SyllabusNode {
    char subject[MAX_SUBJECT];
    char units[MAX_UNITS];
    BookNode* books;
    struct SyllabusNode* next;
} SyllabusNode;

extern SyllabusNode* syllabusListHead;

void addSyllabus();
void addBookToSyllabus();
void viewAllSyllabus();
void searchSyllabusBySubject();
void viewBooksForSubject();

// ADD THESE ↓↓↓
void initializeSyllabus();
void cleanupSyllabus();
void saveAllSyllabus();
void saveAllBooks();

#endif
