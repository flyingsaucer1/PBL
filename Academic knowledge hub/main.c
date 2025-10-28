
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "auth.h"
#include "faculty.h"
#include "doubt.h"
#include "notes.h"
#include "quiz.h"
#include "community.h"

// Syllabus function prototypes
void addSyllabus();
void addBookToSyllabus();
void viewAllSyllabus();
void searchSyllabusBySubject();
void viewBooksForSubject();
void initializeSyllabus();
void cleanupSyllabus();

void studentMenu(User* currentUser);
void facultyMenu(User* currentUser);
void adminMenu(User* currentUser);

int main() {
    printf("\n========================================\n");
    printf("     ACADEMIC KNOWLEDGE HUB\n");
    printf("   Graphic Era University\n");
    printf("========================================\n");

    initializeSystem();
    initializeNotes();
    initializeCommunity();
    initializeDoubts();
    initializeFaculty();
    initializeSyllabus();
    initializeQuiz();

    int choice;
    User* currentUser = NULL;

    while(1) {
        displayMainMenu();

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1:
                currentUser = login();
                if(currentUser != NULL) {
                    if(strcmp(currentUser->role, "student") == 0) {
                        studentMenu(currentUser);
                    } else if(strcmp(currentUser->role, "faculty") == 0) {
                        facultyMenu(currentUser);
                    } else if(strcmp(currentUser->role, "admin") == 0) {
                        adminMenu(currentUser);
                    }
                    currentUser = NULL;
                }
                break;

            case 2:
                registerUserTerminal();
                break;

            case 3:
                cleanupNotes();
                cleanupCommunity();
                cleanupDoubts();
                cleanupFaculty();
                cleanupSyllabus();
                cleanupQuiz();
                cleanupSystem();
                printf("\n Thank you for using Smart Study Hub!\n");
                printf("   Graphic Era University\n");
                printf("========================================\n");
                exit(0);

            default:
                printf("\n Invalid choice! Please select 1-3.\n");
        }
    }

    return 0;
}

void studentMenu(User* currentUser) {
    int choice;
    while(1) {
        printf("\n========================================\n");
        printf("       STUDENT DASHBOARD\n");
        printf("      Welcome, %s!\n", currentUser->username);
        printf("========================================\n");
        printf("  LEARNING:\n");
        printf("1. View All Notes/Lectures\n");
        printf("2. Search Notes by Subject\n");
        printf("3. Search Notes by Title\n");
        printf("\n  ASSESSMENT:\n");
        printf("4. View Available Quizzes\n");
        printf("5. Take a Quiz\n");
        printf("\n  HELP:\n");
        printf("6. Post a Doubt\n");
        printf("7. View My Doubts\n");
        printf("8. View Solved Doubts\n");
        printf("\n  INFORMATION:\n");
        printf("9. View All Faculty\n");
        printf("10. Search Faculty by Department\n");
        printf("11. Search Faculty by Subject\n");
        printf("\n  COMMUNITY:\n");
        printf("12. View Community Posts\n");
        printf("13. Create a Post\n");
        printf("14. Add Comment to Post\n");
        printf("15. View Post with Comments\n");
        printf("\n  SYLLABUS:\n");
        printf("16. View All Syllabus\n");
        printf("17. Search Syllabus by Subject\n");
        printf("18. View Reference Books\n");
        printf("\n19.  Logout\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n Invalid input!\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1: viewAllNotes(); break;
            case 2: searchNoteBySubject(); break;
            case 3: searchNoteByTitle(); break;
            case 4: viewAllQuizzes(); break;
            case 5: takeQuiz(currentUser->username); getchar();break;
            case 6: postDoubt(currentUser->username); break;
            case 7: viewMyDoubts(currentUser->username); break;
            case 8: viewSolvedDoubts(); break;
            case 9: viewAllFaculty(); break;
            case 10: searchFacultyByDept(); break;
            case 11: searchFacultyBySubject(); break;
            case 12: viewAllPosts(); break;
            case 13: createPost(currentUser->username); break;
            case 14: addComment(currentUser->username); break;
            case 15: viewPostWithComments(); break;
            case 16: viewAllSyllabus(); break;
            case 17: searchSyllabusBySubject(); break;
            case 18: viewBooksForSubject(); break;
            case 19:
                printf("\n Logging out...\n");
                return;
            default:
                printf("\n Invalid choice!\n");
        }

        printf("\n⏎ Press Enter to continue...");
        getchar();
    }
}

void facultyMenu(User* currentUser) {
    int choice;
    while(1) {
        printf("\n========================================\n");
        printf("        FACULTY DASHBOARD\n");
        printf("      Welcome, %s!\n", currentUser->username);
        printf("========================================\n");
        printf("  CONTENT MANAGEMENT:\n");
        printf("1. Upload New Note/Lecture\n");
        printf("2. View All Notes\n");
        printf("3. Search Notes by Subject\n");
        printf("\n  QUIZ MANAGEMENT:\n");
        printf("4. Create New Quiz\n");
        printf("5. View All Quizzes\n");
        printf("\n  SYLLABUS MANAGEMENT:\n");
        printf("6. Add New Syllabus\n");
        printf("7. Add Reference Book\n");
        printf("8. View All Syllabus\n");
        printf("9. Search Syllabus by Subject\n");
        printf("\n  DOUBT RESOLUTION:\n");
        printf("10. View Pending Doubts\n");
        printf("11. Solve a Doubt\n");
        printf("12. View Solved Doubts\n");
        printf("\n  FACULTY INFO:\n");
        printf("13. View All Faculty\n");
        printf("14. Search Faculty by Department\n");
        printf("\n  COMMUNITY:\n");
        printf("15. View Community Posts\n");
        printf("16. Create a Post\n");
        printf("17. Add Comment to Post\n");
        printf("18. View Post with Comments\n");
        printf("\n19.  Logout\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n Invalid input!\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1: uploadNote(); break;
            case 2: viewAllNotes(); break;
            case 3: searchNoteBySubject(); break;
            case 4: createQuiz(); break;
            case 5: viewAllQuizzes(); break;
            case 6: addSyllabus(); break;
            case 7: addBookToSyllabus(); break;
            case 8: viewAllSyllabus(); break;
            case 9: searchSyllabusBySubject(); break;
            case 10: viewPendingDoubts(); break;
            case 11: solveDoubt(currentUser->username); break;
            case 12: viewSolvedDoubts(); break;
            case 13: viewAllFaculty(); break;
            case 14: searchFacultyByDept(); break;
            case 15: viewAllPosts(); break;
            case 16: createPost(currentUser->username); break;
            case 17: addComment(currentUser->username); break;
            case 18: viewPostWithComments(); break;
            case 19:
                printf("\n Logging out...\n");
                return;
            default:
                printf("\n Invalid choice!\n");
        }

        printf("\n⏎ Press Enter to continue...");
        getchar();
    }
}

void adminMenu(User* currentUser) {
    int choice;
    while(1) {
        printf("\n========================================\n");
        printf("       ADMIN DASHBOARD\n");
        printf("      Welcome, %s!\n", currentUser->username);
        printf("========================================\n");
        printf("  USER MANAGEMENT:\n");
        printf("1. Approve Pending Accounts\n");
        printf("\n  FACULTY MANAGEMENT:\n");
        printf("2. Add Faculty Record\n");
        printf("3. Remove Faculty Record\n");
        printf("4. View All Faculty\n");
        printf("5. Search Faculty by ID\n");
        printf("6. Search Faculty by Department\n");
        printf("7. Search Faculty by Subject\n");
        printf("\n  SYSTEM MONITORING:\n");
        printf("8. View All Notes\n");
        printf("9. View All Quizzes\n");
        printf("10. View All Doubts\n");
        printf("11. View Community Posts\n");
        printf("12. View All Syllabus\n");
        printf("\n13.  Logout\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n Invalid input!\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1: approveAccounts(); break;
            case 2: addFaculty(); break;
            case 3: removeFaculty(); break;
            case 4: viewAllFaculty(); break;
            case 5: searchFacultyById(); break;
            case 6: searchFacultyByDept(); break;
            case 7: searchFacultyBySubject(); break;
            case 8: viewAllNotes(); break;
            case 9: viewAllQuizzes(); break;
            case 10:
                printf("\n--- PENDING DOUBTS ---\n");
                viewPendingDoubts();
                printf("\n--- SOLVED DOUBTS ---\n");
                viewSolvedDoubts();
                break;
            case 11: viewAllPosts(); break;
            case 12: viewAllSyllabus(); break;
            case 13:
                printf("\n Logging out...\n");
                return;
            default:
                printf("\n Invalid choice!\n");
        }

        printf("\n⏎ Press Enter to continue...");
        getchar();
    }
}