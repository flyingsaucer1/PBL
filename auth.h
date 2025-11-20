// ============================================
// FILE: auth.h
// Authentication Module Header
// ============================================

#ifndef AUTH_H
#define AUTH_H

#define TABLE_SIZE 100

typedef struct User {
    char username[50];
    char password[100];
    char email[100];
    char role[20];
    int isApproved;
    struct User *next;
} User;

extern User *hashTable[TABLE_SIZE];

void initializeSystem();
int hash(char *username);
void clearInputBuffer();

User* createUser(char username[], char password[], char email[], char role[], int isApproved);
void insertUser(User *user);
User* findUser(char username[], char password[]);
User* findUserByUsername(char username[]);

int registerUser(char username[], char password[], char email[], char role[]);
int loginUser(char username[], char password[]);
void registerUserTerminal();
User* login();

int validateEmail(char *email);
void getUserInfo(char username[], char *role, char *email, int *isApproved);

int approveUser(char username[]);
void approveAccounts();
void listPendingUsers(char *result);

void loadAccounts();
void saveAccount(User *acc);
void saveAllAccounts();

void displayMainMenu();
void cleanupSystem();

#endif // AUTH_H
