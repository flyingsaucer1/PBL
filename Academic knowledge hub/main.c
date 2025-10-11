#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =================== Include all relevant headers ===================
#include "structures.h"
#include "auth.h"       // initializeSystem, displayMainMenu, clearInputBuffer, registerUserTerminal, login
#include "faculty.h"    // faculty-related functions
#include "syllabus.h"   // syllabus functions
#include "doubt.h"      // doubts functions
#include "notes.h"      // notes functions
#include "quiz.h"       // quiz functions
#include "community.h"  // community functions

// =================== Function Prototypes ===================
void studentMenu(User* currentUser);
void facultyMenu(User* currentUser);
void adminMenu(User* currentUser);

// =================== MAIN FUNCTION ===================
int main() {
    initializeSystem();

    int choice;
    User* currentUser = NULL;

    printf("\n========================================\n");
    printf("   🎓 SMART STUDY HUB\n");
    printf("   Graphic Era University\n");
    printf("========================================\n");

    while(1) {
        displayMainMenu();

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n❌ Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1: // Login
                currentUser = login();
                if(currentUser != NULL) {
                    if(strcmp(currentUser->role, "student") == 0) {
                        studentMenu(currentUser);
                    } else if(strcmp(currentUser->role, "faculty") == 0) {
                        facultyMenu(currentUser);
                    } else if(strcmp(currentUser->role, "admin") == 0) {
                        adminMenu(currentUser);
                    }
                    free(currentUser);
                    currentUser = NULL;
                }
                break;

            case 2: // Register
                registerUserTerminal();
                break;

            case 3: // Exit
                printf("\n🎓 Thank you for using Smart Study Hub!\n");
                printf("   Graphic Era University\n");
                printf("========================================\n");
                exit(0);

            default:
                printf("\n❌ Invalid choice! Please select 1-3.\n");
        }
    }

    return 0;
}

// =================== STUDENT MENU ===================
void studentMenu(User* currentUser) {
    int choice;
    while(1) {
        printf("\n========================================\n");
        printf("      👨‍🎓 STUDENT DASHBOARD\n");
        printf("      Welcome, %s!\n", currentUser->username);
        printf("========================================\n");
        printf("📚 LEARNING:\n");
        printf("1. View All Notes/Lectures\n");
        printf("2. Search Notes by Subject\n");
        printf("3. Search Notes by Title\n");
        printf("\n✍️ ASSESSMENT:\n");
        printf("4. View Available Quizzes\n");
        printf("5. Take a Quiz\n");
        printf("\n❓ HELP:\n");
        printf("6. Post a Doubt\n");
        printf("7. View My Doubts\n");
        printf("8. View Solved Doubts\n");
        printf("\n👨‍🏫 INFORMATION:\n");
        printf("9. View All Faculty\n");
        printf("10. Search Faculty by Department\n");
        printf("11. Search Faculty by Subject\n");
        printf("\n💬 COMMUNITY:\n");
        printf("12. View Community Posts\n");
        printf("13. Create a Post\n");
        printf("14. Add Comment to Post\n");
        printf("15. View Post with Comments\n");
        printf("\n📖 SYLLABUS:\n");
        printf("16. View All Syllabus\n");
        printf("17. Search Syllabus by Subject\n");
        printf("18. View Reference Books\n");
        printf("\n19. 🚪 Logout\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n❌ Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1: viewAllNotes(); break;
            case 2: searchNoteBySubject(); break;
            case 3: searchNoteByTitle(); break;
            case 4: viewAllQuizzes(); break;
            case 5: takeQuiz(currentUser->username); break;  // ✅ Fixed
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
                printf("\n👋 Logging out...\n");
                return;
            default:
                printf("\n❌ Invalid choice! Please select 1-19.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}

// =================== FACULTY MENU ===================
void facultyMenu(User* currentUser) {
    printf("\nFaculty menu placeholder (implement your menu here)...\n");
}

// =================== ADMIN MENU ===================
void adminMenu(User* currentUser) {
    printf("\nAdmin menu placeholder (implement your menu here)...\n");
}
