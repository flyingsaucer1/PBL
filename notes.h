// ============================================
// FILE: notes.h
// Notes Module Header
// ============================================

#ifndef NOTES_H
#define NOTES_H

// Backend-facing functions (original ones)
void uploadNote();
void viewAllNotes();
void searchNoteBySubject();
void searchNoteByTitle();
void initializeNotes();
void cleanupNotes();

// Extra helper functions for web API
// (called from notes_api.c, not from user directly)
void addNoteFromData(const char* title,
                     const char* subject,
                     const char* content,
                     const char* faculty);

void printAllNotesAsJson();

#endif // NOTES_H
