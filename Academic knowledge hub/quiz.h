#ifndef QUIZ_H
#define QUIZ_H

void createQuiz();
void viewAllQuizzes();
void takeQuiz(char* username);
void deleteAllQuestions();
void viewQuizSummary();
void initializeQuiz();
void cleanupQuiz();

/* ===== Extra helpers for web / quiz_api.exe ===== */

// Prints: [ {"title":"...","totalQuestions":N}, ... ]
void printAllQuizzesAsJson();

// Prints full quiz (including questions & correct option) or `null`
void printQuizAsJson(const char* title);

// Add (or create+add) a question to a quiz programmatically.
// Returns 1 on success, 0 on failure.
int addQuestionProgrammatic(const char* quizTitle,
                            const char* question,
                            const char* optionA,
                            const char* optionB,
                            const char* optionC,
                            const char* optionD,
                            char correct);

#endif
