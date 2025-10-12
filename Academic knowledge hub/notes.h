#ifndef NOTES_H
#define NOTES_H

#include "structures.h"

extern Note* noteListHead;

void uploadNote();
void viewAllNotes();
void searchNoteBySubject();
void searchNoteByTitle();

#endif