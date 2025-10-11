#ifndef AUTH_H
#define AUTH_H

#include "structures.h"

#define DB_FILE "accounts.txt"

// Terminal functions
void initializeSystem();
void displayMainMenu();
void clearInputBuffer();
void registerUserTerminal();
User* login();

// Backend functions
int registerUser(char username[], char password[], char email[], char role[]);
int loginUser(char username[], char password[]);
User* createUser(char username[], char password[], char email[], char role[], int isApproved);
void insertUser(User* user);
User* findUser(char username[], char password[]);
User* findUserByUsername(char username[]);
void loadAccounts();
void saveAccount(User* acc);
void saveAllAccounts();
int approveUser(char username[]);
void listPendingUsers(char *result);
void approveAccounts();
void getUserInfo(char username[], char *role, char *email, int *isApproved);

#endif
