#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "quiz.h"

#define QUIZ_FILE "quizzes.txt"

typedef struct Question {
    char question[200];
    char optionA[50], optionB[50], optionC[50], optionD[50];
    char correct;
    struct Question* next;
} Question;

typedef struct Quiz {
    char title[100];
    int totalQuestions;
    Question *questionsHead;
    struct Quiz* next;
} Quiz;

Quiz* quizListHead = NULL;

/* =============================
   Internal helpers
   ============================= */

static Quiz* findQuizByTitle(const char* title) {
    Quiz* q = quizListHead;
    while (q) {
        if (strcasecmp(q->title, title) == 0) {
            return q;
        }
        q = q->next;
    }
    return NULL;
}

static void addQuizProgrammatic(const char* title) {
    if (findQuizByTitle(title) != NULL) return;

    Quiz* newQuiz = (Quiz*)malloc(sizeof(Quiz));
    if (!newQuiz) return;

    strncpy(newQuiz->title, title, sizeof(newQuiz->title) - 1);
    newQuiz->title[sizeof(newQuiz->title) - 1] = '\0';
    newQuiz->totalQuestions = 0;
    newQuiz->questionsHead = NULL;
    newQuiz->next = NULL;

    if (!quizListHead) {
        quizListHead = newQuiz;
    } else {
        Quiz* temp = quizListHead;
        while (temp->next) temp = temp->next;
        temp->next = newQuiz;
    }
}

/* =============================
   File I/O
   ============================= */

void loadQuiz() {
    FILE* fp = fopen(QUIZ_FILE, "r");
    if (!fp) {
        return;
    }

    char line[500];
    Quiz* currentQuiz = NULL;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "QUIZ:", 5) == 0) {
            Quiz* newQuiz = (Quiz*)malloc(sizeof(Quiz));
            if (!newQuiz) continue;

            strncpy(newQuiz->title, line + 5, 99);
            newQuiz->title[99] = '\0';
            newQuiz->totalQuestions = 0;
            newQuiz->questionsHead = NULL;
            newQuiz->next = NULL;

            if (!quizListHead) {
                quizListHead = newQuiz;
            } else {
                Quiz* temp = quizListHead;
                while (temp->next) temp = temp->next;
                temp->next = newQuiz;
            }
            currentQuiz = newQuiz;
        }
        else if (strchr(line, '|') && currentQuiz) {
            Question* newQ = (Question*)malloc(sizeof(Question));
            if (!newQ) continue;

            char* token;
            char lineCopy[500];
            strcpy(lineCopy, line);

            token = strtok(lineCopy, "|");
            if (token) strncpy(newQ->question, token, 199);
            newQ->question[199] = '\0';
            
            token = strtok(NULL, "|");
            if (token) strncpy(newQ->optionA, token, 49);
            newQ->optionA[49] = '\0';
            
            token = strtok(NULL, "|");
            if (token) strncpy(newQ->optionB, token, 49);
            newQ->optionB[49] = '\0';
            
            token = strtok(NULL, "|");
            if (token) strncpy(newQ->optionC, token, 49);
            newQ->optionC[49] = '\0';
            
            token = strtok(NULL, "|");
            if (token) strncpy(newQ->optionD, token, 49);
            newQ->optionD[49] = '\0';
            
            token = strtok(NULL, "|");
            if (token) {
                newQ->correct = toupper((unsigned char)token[0]);
            }

            newQ->next = NULL;

            if (!currentQuiz->questionsHead) {
                currentQuiz->questionsHead = newQ;
            } else {
                Question* temp = currentQuiz->questionsHead;
                while (temp->next) temp = temp->next;
                temp->next = newQ;
            }
            currentQuiz->totalQuestions++;
        }
    }

    fclose(fp);
}

void saveAllQuiz() {
    FILE* fp = fopen(QUIZ_FILE, "w");
    if (!fp) {
        printf(" Error: Cannot save quizzes!\n");
        return;
    }

    Quiz* quiz = quizListHead;
    while (quiz) {
        fprintf(fp, "QUIZ:%s\n", quiz->title);
        
        Question* q = quiz->questionsHead;
        while (q) {
            fprintf(fp, "%s|%s|%s|%s|%s|%c\n",
                    q->question, q->optionA, q->optionB,
                    q->optionC, q->optionD, q->correct);
            q = q->next;
        }
        fprintf(fp, "\n");
        quiz = quiz->next;
    }

    fflush(fp);
    fclose(fp);
}

/* =============================
   Helpers for web / quiz_api.exe
   ============================= */

int addQuestionProgrammatic(const char* quizTitle,
                            const char* question,
                            const char* optionA,
                            const char* optionB,
                            const char* optionC,
                            const char* optionD,
                            char correct) {
    if (!quizTitle || !question || !optionA || !optionB || !optionC || !optionD) {
        return 0;
    }

    Quiz* quiz = findQuizByTitle(quizTitle);
    if (!quiz) {
        addQuizProgrammatic(quizTitle);
        quiz = findQuizByTitle(quizTitle);
        if (!quiz) return 0;
    }

    Question* newQ = (Question*)malloc(sizeof(Question));
    if (!newQ) return 0;

    strncpy(newQ->question, question, sizeof(newQ->question) - 1);
    newQ->question[sizeof(newQ->question) - 1] = '\0';

    strncpy(newQ->optionA, optionA, sizeof(newQ->optionA) - 1);
    newQ->optionA[sizeof(newQ->optionA) - 1] = '\0';

    strncpy(newQ->optionB, optionB, sizeof(newQ->optionB) - 1);
    newQ->optionB[sizeof(newQ->optionB) - 1] = '\0';

    strncpy(newQ->optionC, optionC, sizeof(newQ->optionC) - 1);
    newQ->optionC[sizeof(newQ->optionC) - 1] = '\0';

    strncpy(newQ->optionD, optionD, sizeof(newQ->optionD) - 1);
    newQ->optionD[sizeof(newQ->optionD) - 1] = '\0';

    newQ->correct = toupper((unsigned char)correct);
    newQ->next = NULL;

    if (!quiz->questionsHead) {
        quiz->questionsHead = newQ;
    } else {
        Question* temp = quiz->questionsHead;
        while (temp->next) temp = temp->next;
        temp->next = newQ;
    }

    quiz->totalQuestions++;
    saveAllQuiz();
    return 1;
}

void printAllQuizzesAsJson() {
    Quiz* quiz = quizListHead;
    printf("[\n");
    int first = 1;
    while (quiz) {
        if (!first) {
            printf(",\n");
        }
        first = 0;
        printf("  {\"title\":\"%s\",\"totalQuestions\":%d}",
               quiz->title, quiz->totalQuestions);
        quiz = quiz->next;
    }
    printf("\n]\n");
}

void printQuizAsJson(const char* title) {
    if (!title) {
        printf("null\n");
        return;
    }

    Quiz* quiz = findQuizByTitle(title);
    if (!quiz) {
        printf("null\n");
        return;
    }

    printf("{\"title\":\"%s\",\"totalQuestions\":%d,\"questions\":[",
           quiz->title, quiz->totalQuestions);

    Question* q = quiz->questionsHead;
    int first = 1;
    while (q) {
        if (!first) printf(",");
        first = 0;
        printf("{\"question\":\"%s\",\"optionA\":\"%s\",\"optionB\":\"%s\","
               "\"optionC\":\"%s\",\"optionD\":\"%s\",\"correct\":\"%c\"}",
               q->question, q->optionA, q->optionB,
               q->optionC, q->optionD, q->correct);
        q = q->next;
    }
    printf("]}\n");
}

/* =============================
   Original console functions
   ============================= */

void displayAllQuestions() {
    if (!quizListHead) {
        printf("\n No quizzes available.\n");
        return;
    }

    printf("\n========================================\n");
    printf("      AVAILABLE QUIZZES\n");
    printf("========================================\n");
    
    Quiz* quiz = quizListHead;
    int count = 1;
    
    while (quiz) {
        printf("\n%d.  %s\n", count, quiz->title);
        printf("   Total Questions: %d\n", quiz->totalQuestions);
        printf("----------------------------------------\n");
        quiz = quiz->next;
        count++;
    }
}

void viewAllQuizzes() {
    displayAllQuestions();
}

void createQuiz() {
    Quiz* newQuiz = (Quiz*)malloc(sizeof(Quiz));
    if (!newQuiz) {
        printf(" Memory allocation failed!\n");
        return;
    }

    printf("\n========================================\n");
    printf("      CREATE NEW QUIZ\n");
    printf("========================================\n");
    
    printf("Enter Quiz Title/Subject: ");
    fgets(newQuiz->title, 100, stdin);
    newQuiz->title[strcspn(newQuiz->title, "\n")] = 0;
    
    newQuiz->totalQuestions = 0;
    newQuiz->questionsHead = NULL;
    newQuiz->next = NULL;

    int n;
    printf("Enter number of questions to add: ");
    scanf("%d", &n);
    getchar();

    Question* lastQuestion = NULL;

    for (int i = 0; i < n; i++) {
        Question* newQ = (Question*)malloc(sizeof(Question));
        if (!newQ) continue;

        char q[200], a[50], b[50], c[50], d[50], ans;
        
        printf("\n--- Question %d ---\n", i + 1);
        printf("Enter Question: ");
        fgets(q, sizeof(q), stdin); 
        q[strcspn(q, "\n")] = 0;
        
        printf("Option A: "); 
        fgets(a, sizeof(a), stdin); 
        a[strcspn(a, "\n")] = 0;
        
        printf("Option B: "); 
        fgets(b, sizeof(b), stdin); 
        b[strcspn(b, "\n")] = 0;
        
        printf("Option C: "); 
        fgets(c, sizeof(c), stdin); 
        c[strcspn(c, "\n")] = 0;
        
        printf("Option D: "); 
        fgets(d, sizeof(d), stdin); 
        d[strcspn(d, "\n")] = 0;
        
        printf("Correct Option (A/B/C/D): ");
        scanf(" %c", &ans);
        getchar();

        strcpy(newQ->question, q);
        strcpy(newQ->optionA, a);
        strcpy(newQ->optionB, b);
        strcpy(newQ->optionC, c);
        strcpy(newQ->optionD, d);
        newQ->correct = toupper((unsigned char)ans);
        newQ->next = NULL;

        if (!newQuiz->questionsHead) {
            newQuiz->questionsHead = newQ;
        } else {
            lastQuestion->next = newQ;
        }
        lastQuestion = newQ;
        newQuiz->totalQuestions++;
    }

    if (!quizListHead) {
        quizListHead = newQuiz;
    } else {
        Quiz* temp = quizListHead;
        while (temp->next) temp = temp->next;
        temp->next = newQuiz;
    }

    saveAllQuiz();
    printf("\n Quiz '%s' created successfully with %d questions!\n", newQuiz->title, n);
}

void deleteAllQuestions() {
    Quiz* quiz = quizListHead;
    while (quiz) {
        Question* q = quiz->questionsHead;
        while (q) {
            Question* next = q->next;
            free(q);
            q = next;
        }
        Quiz* nextQuiz = quiz->next;
        free(quiz);
        quiz = nextQuiz;
    }
    quizListHead = NULL;
    
    FILE* fp = fopen(QUIZ_FILE, "w");
    if (fp) {
        fclose(fp);
    }
    
    printf("\n All quizzes deleted successfully!\n");
}

void takeQuiz(char* username) {
    if (!quizListHead) {
        printf("\n No quizzes available yet!\n");
        return;
    }

    displayAllQuestions();
    
    char chosenTitle[100];
    printf("\nEnter quiz title to take: ");
    fgets(chosenTitle, 100, stdin);
    chosenTitle[strcspn(chosenTitle, "\n")] = 0;

    Quiz* selectedQuiz = quizListHead;
    while (selectedQuiz) {
        if (strcasecmp(selectedQuiz->title, chosenTitle) == 0) {
            break;
        }
        selectedQuiz = selectedQuiz->next;
    }

    if (!selectedQuiz) {
        printf("\n Quiz not found! Check the title and try again.\n");
        return;
    }

    int count = selectedQuiz->totalQuestions;
    if (count <= 0) {
        printf("\n This quiz has no questions.\n");
        return;
    }

    Question* arr[100];
    Question* temp = selectedQuiz->questionsHead;

    int i = 0;
    while (temp && i < 100) {
        arr[i++] = temp;
        temp = temp->next;
    }
    count = i;

    srand((unsigned int)time(NULL));
    for (int j = 0; j < count; j++) {
        int r = rand() % count;
        Question* t = arr[j];
        arr[j] = arr[r];
        arr[r] = t;
    }

    int score = 0;
    char choice;

    printf("\n========================================\n");
    printf("      QUIZ: %s\n", selectedQuiz->title);
    printf("      Total Questions: %d\n", count);
    printf("========================================\n");

    for (int k = 0; k < count; k++) {
        printf("\nQ%d: %s\n", k + 1, arr[k]->question);
        printf("A) %s\n", arr[k]->optionA);
        printf("B) %s\n", arr[k]->optionB);
        printf("C) %s\n", arr[k]->optionC);
        printf("D) %s\n", arr[k]->optionD);
        printf("Enter choice: ");
        scanf(" %c", &choice);
        choice = toupper((unsigned char)choice);
        if (choice == arr[k]->correct) {
            score++;
            printf("✓ Correct!\n");
        } else {
            printf("✗ Wrong! Correct answer: %c\n", arr[k]->correct);
        }
    }

    printf("\n========================================\n");
    printf("      QUIZ COMPLETED!\n");
    printf("========================================\n");
    printf(" Quiz: %s\n", selectedQuiz->title);
    printf(" Student: %s\n", username);
    printf(" Score: %d out of %d\n", score, count);
    printf(" Percentage: %.2f%%\n", (score * 100.0) / count);
    printf("========================================\n");
}

void viewQuizSummary() {
    printf("\n========================================\n");
    printf("      QUIZ SUMMARY\n");
    printf("========================================\n");
    
    if (!quizListHead) {
        printf("No quizzes available.\n");
    } else {
        Quiz* quiz = quizListHead;
        int totalQuizzes = 0;
        int totalQuestions = 0;
        
        while (quiz) {
            totalQuizzes++;
            totalQuestions += quiz->totalQuestions;
            quiz = quiz->next;
        }
        
        printf("Total Quizzes: %d\n", totalQuizzes);
        printf("Total Questions: %d\n", totalQuestions);
    }
    printf("========================================\n");
}

void initializeQuiz() {
    loadQuiz();
}

void cleanupQuiz() {
    Quiz* quiz = quizListHead;
    while (quiz) {
        Question* q = quiz->questionsHead;
        while (q) {
            Question* toFree = q;
            q = q->next;
            free(toFree);
        }
        Quiz* toFree = quiz;
        quiz = quiz->next;
        free(toFree);
    }
    quizListHead = NULL;
}
