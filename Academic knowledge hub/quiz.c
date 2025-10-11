#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "quiz.h"

// ====================== Structures ======================

// Structure for each question
typedef struct Question {
    char question[200];
    char optionA[50], optionB[50], optionC[50], optionD[50];
    char correct;
    struct Question* next;
} Question;

// Structure for quiz queue
typedef struct Queue {
    Question *front, *rear;
    int totalQuestions;
} Queue;

// Global quiz queue
Queue quizQueue = {NULL, NULL, 0};

// ====================== Queue Operations ======================

void enqueueQuestion(char q[], char a[], char b[], char c[], char d[], char ans) {
    Question* newQ = (Question*)malloc(sizeof(Question));
    strcpy(newQ->question, q);
    strcpy(newQ->optionA, a);
    strcpy(newQ->optionB, b);
    strcpy(newQ->optionC, c);
    strcpy(newQ->optionD, d);
    newQ->correct = toupper(ans);
    newQ->next = NULL;

    if (quizQueue.rear == NULL)
        quizQueue.front = quizQueue.rear = newQ;
    else {
        quizQueue.rear->next = newQ;
        quizQueue.rear = newQ;
    }

    quizQueue.totalQuestions++;
}

void displayAllQuestions() {
    Question* temp = quizQueue.front;
    if (!temp) {
        printf("\nNo questions available.\n");
        return;
    }

    printf("\n--- All Stored Questions ---\n");
    int i = 1;
    while (temp) {
        printf("\nQ%d: %s\n", i++, temp->question);
        printf("A) %s\nB) %s\nC) %s\nD) %s\nCorrect: %c\n",
               temp->optionA, temp->optionB, temp->optionC, temp->optionD, temp->correct);
        temp = temp->next;
    }
}
#include "quiz.h"

void viewAllQuizzes() {
    // For now, just display all questions as a placeholder
    printf("\n--- All Quizzes ---\n");
    displayAllQuestions();
}


// ====================== Faculty Functions ======================

// Add new questions
void createQuiz() {
    int n;
    printf("\nEnter number of questions to add: ");
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; i++) {
        char q[200], a[50], b[50], c[50], d[50], ans;
        printf("\nEnter Question %d: ", i + 1);
        fgets(q, sizeof(q), stdin); q[strcspn(q, "\n")] = 0;
        printf("Option A: "); fgets(a, sizeof(a), stdin); a[strcspn(a, "\n")] = 0;
        printf("Option B: "); fgets(b, sizeof(b), stdin); b[strcspn(b, "\n")] = 0;
        printf("Option C: "); fgets(c, sizeof(c), stdin); c[strcspn(c, "\n")] = 0;
        printf("Option D: "); fgets(d, sizeof(d), stdin); d[strcspn(d, "\n")] = 0;
        printf("Correct Option (A/B/C/D): ");
        scanf(" %c", &ans);
        getchar();

        enqueueQuestion(q, a, b, c, d, ans);
    }

    printf("\nQuiz created successfully!\n");
}

// Delete all quiz questions
void deleteAllQuestions() {
    Question* temp = quizQueue.front;
    while (temp) {
        Question* next = temp->next;
        free(temp);
        temp = next;
    }
    quizQueue.front = quizQueue.rear = NULL;
    quizQueue.totalQuestions = 0;
    printf("\nAll quiz questions deleted successfully!\n");
}

// ====================== Student Functions ======================

void takeQuiz(char* username) {
    if (quizQueue.front == NULL) {
        printf("\nNo quiz available yet!\n");
        return;
    }

    int count = quizQueue.totalQuestions;
    Question* arr[100];
    Question* temp = quizQueue.front;

    int i = 0;
    while (temp && i < 100) {
        arr[i++] = temp;
        temp = temp->next;
    }

    // Random shuffle
    for (int j = 0; j < count; j++) {
        int r = rand() % count;
        Question* t = arr[j];
        arr[j] = arr[r];
        arr[r] = t;
    }

    int score = 0;
    char choice;

    for (int k = 0; k < count; k++) {
        printf("\nQ%d: %s\n", k + 1, arr[k]->question);
        printf("A) %s\nB) %s\nC) %s\nD) %s\n", arr[k]->optionA, arr[k]->optionB, arr[k]->optionC, arr[k]->optionD);
        printf("Enter choice: ");
        scanf(" %c", &choice);
        choice = toupper(choice);
        if (choice == arr[k]->correct)
            score++;
    }

    printf("\nQuiz Completed! You scored %d out of %d.\n", score, count);
}

// ====================== Admin Functions ======================

void viewQuizSummary() {
    printf("\n--- Quiz Summary ---\n");
    printf("Total Questions: %d\n", quizQueue.totalQuestions);
    printf("Queue Status: %s\n", quizQueue.front ? "Active" : "Empty");
    if (quizQueue.front) {
        printf("First Question: %s\n", quizQueue.front->question);
    }
}
