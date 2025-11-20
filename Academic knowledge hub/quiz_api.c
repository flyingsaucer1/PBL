// ============================================
// FILE: quiz_api.c
// CLI wrapper for Quiz module (for Node)
//
// Commands:
//
//   quiz_api list
//      -> prints JSON array of quizzes
//
//   quiz_api get
//      (stdin: quizTitle)
//      -> prints JSON for one quiz or null
//
//   quiz_api addQuestion
//      (stdin: quizTitle|question|A|B|C|D|correct)
//      -> prints {"success":true/false,"message":""}
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "quiz.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: quiz_api <list|get|addQuestion>\n");
        return 1;
    }

    initializeQuiz();

    if (strcmp(argv[1], "list") == 0) {
        printAllQuizzesAsJson();
    }
    else if (strcmp(argv[1], "get") == 0) {
        char title[100];
        if (!fgets(title, sizeof(title), stdin)) {
            printf("null\n");
            cleanupQuiz();
            return 0;
        }
        title[strcspn(title, "\r\n")] = 0;
        printQuizAsJson(title);
    }
    else if (strcmp(argv[1], "addQuestion") == 0) {
        char line[700];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("{\"success\":false,\"message\":\"No input received.\"}\n");
            cleanupQuiz();
            return 0;
        }

        char *quizTitle, *question, *a, *b, *c, *d, *corrStr;

        quizTitle = strtok(line, "|");
        question  = strtok(NULL, "|");
        a         = strtok(NULL, "|");
        b         = strtok(NULL, "|");
        c         = strtok(NULL, "|");
        d         = strtok(NULL, "|");
        corrStr   = strtok(NULL, "|");

        if (!quizTitle || !question || !a || !b || !c || !d || !corrStr) {
            printf("{\"success\":false,\"message\":\"Invalid input format for addQuestion.\"}\n");
            cleanupQuiz();
            return 0;
        }

        quizTitle[strcspn(quizTitle, "\r\n")] = 0;
        question[strcspn(question, "\r\n")] = 0;
        a[strcspn(a, "\r\n")] = 0;
        b[strcspn(b, "\r\n")] = 0;
        c[strcspn(c, "\r\n")] = 0;
        d[strcspn(d, "\r\n")] = 0;
        corrStr[strcspn(corrStr, "\r\n")] = 0;

        char corr = toupper((unsigned char)corrStr[0]);

        int ok = addQuestionProgrammatic(quizTitle, question, a, b, c, d, corr);

        if (ok) {
            printf("{\"success\":true,\"message\":\"Question added.\"}\n");
        } else {
            printf("{\"success\":false,\"message\":\"Failed to add question.\"}\n");
        }
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        cleanupQuiz();
        return 1;
    }

    cleanupQuiz();
    return 0;
}
