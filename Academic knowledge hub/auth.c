#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"

#define DB_FILE "accounts.txt"
#define TEMP_FILE "accounts_temp.txt"

User *hashTable[TABLE_SIZE];

int validateEmail(char *email) {
    char *domain = strstr(email, "@geu.ac.in");
    if (domain == NULL) return 0;
    if (strlen(domain) == 10) return 1;
    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int hash(char *username) {
    unsigned long hash = 5381;
    int c;
    while ((c = *username++))
        hash = ((hash << 5) + hash) + c;
    return (int)(hash % TABLE_SIZE);
}

void loadAccounts() {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) {
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        User *acc = (User*)malloc(sizeof(User));
        if (!acc) continue;

        char *token;
        char lineCopy[512];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, ",");
        if (!token) { free(acc); continue; }
        strncpy(acc->username, token, 49);
        acc->username[49] = '\0';

        token = strtok(NULL, ",");
        if (!token) { free(acc); continue; }
        strncpy(acc->password, token, 99);
        acc->password[99] = '\0';

        token = strtok(NULL, ",");
        if (!token) { free(acc); continue; }
        strncpy(acc->email, token, 99);
        acc->email[99] = '\0';

        token = strtok(NULL, ",");
        if (!token) { free(acc); continue; }
        strncpy(acc->role, token, 19);
        acc->role[19] = '\0';

        token = strtok(NULL, ",");
        if (!token) { free(acc); continue; }
        acc->isApproved = atoi(token);

        int index = hash(acc->username);
        acc->next = hashTable[index];
        hashTable[index] = acc;
    }

    fclose(fp);
}

void saveAccount(User *acc) {
    FILE *fp = fopen(DB_FILE, "a");
    if (!fp) {
        printf(" Error: Cannot open accounts file!\n");
        return;
    }

    fprintf(fp, "%s,%s,%s,%s,%d\n",
            acc->username, acc->password, acc->email, acc->role, acc->isApproved);
    fflush(fp);
    fclose(fp);
}

void saveAllAccounts() {
    FILE *fp = fopen(TEMP_FILE, "w");
    if (!fp) {
        printf(" Error: Cannot create temporary file!\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        User *temp = hashTable[i];
        while (temp != NULL) {
            fprintf(fp, "%s,%s,%s,%s,%d\n",
                    temp->username, temp->password, temp->email,
                    temp->role, temp->isApproved);
            temp = temp->next;
        }
    }

    fclose(fp);
    remove(DB_FILE);
    rename(TEMP_FILE, DB_FILE);
}

User* createUser(char username[], char password[], char email[], char role[], int isApproved) {
    User *newUser = (User*) malloc(sizeof(User));
    if (!newUser) return NULL;

    strncpy(newUser->username, username, 49);
    newUser->username[49] = '\0';

    strncpy(newUser->password, password, 99);
    newUser->password[99] = '\0';

    strncpy(newUser->email, email, 99);
    newUser->email[99] = '\0';

    strncpy(newUser->role, role, 19);
    newUser->role[19] = '\0';

    newUser->isApproved = isApproved;
    newUser->next = NULL;
    return newUser;
}

void insertUser(User *user) {
    int index = hash(user->username);
    user->next = hashTable[index];
    hashTable[index] = user;
}

User* findUser(char username[], char password[]) {
    int index = hash(username);
    User *temp = hashTable[index];
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 &&
            strcmp(temp->password, password) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

User* findUserByUsername(char username[]) {
    int index = hash(username);
    User *temp = hashTable[index];
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/*
 * registerUser:
 *  1  -> success
 *  0  -> username exists
 * -1  -> memory fail
 * -2  -> invalid email
 * -3  -> invalid role
 */
int registerUser(char username[], char password[], char email[], char role[]) {
    if (!validateEmail(email)) {
        return -2;
    }

    if (findUserByUsername(username) != NULL) {
        return 0;
    }

    int approved;
    if (strcmp(role, "student") == 0) {
        approved = 1;
    } else if (strcmp(role, "faculty") == 0 || strcmp(role, "admin") == 0) {
        approved = 0;
    } else {
        return -3;
    }

    User *newUser = createUser(username, password, email, role, approved);

    if (!newUser) {
        return -1;
    }

    insertUser(newUser);
    saveAccount(newUser);

    return 1;
}

int loginUser(char username[], char password[]) {
    User *u = findUser(username, password);
    if (u == NULL) return 0;
    if (u->isApproved == 0) return -1;
    return 1;
}

void getUserInfo(char username[], char *role, char *email, int *isApproved) {
    User *u = findUserByUsername(username);
    if (u) {
        strcpy(role, u->role);
        strcpy(email, u->email);
        *isApproved = u->isApproved;
    }
}

int approveUser(char username[]) {
    User *u = findUserByUsername(username);
    if (u && u->isApproved == 0) {
        u->isApproved = 1;
        saveAllAccounts();
        return 1;
    }
    return 0;
}

void listPendingUsers(char *result) {
    strcpy(result, "[");
    int first = 1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        User *temp = hashTable[i];
        while (temp != NULL) {
            if (temp->isApproved == 0) {
                if (!first) strcat(result, ",");
                first = 0;

                char entry[200];
                sprintf(entry, "{\"username\":\"%s\",\"role\":\"%s\",\"email\":\"%s\"}",
                        temp->username, temp->role, temp->email);
                strcat(result, entry);
            }
            temp = temp->next;
        }
    }
    strcat(result, "]");
}

void approveAccounts() {
    printf("\n========================================\n");
    printf("      PENDING ACCOUNTS\n");
    printf("========================================\n");
    int found = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        User *temp = hashTable[i];
        while (temp != NULL) {
            if (temp->isApproved == 0) {
                found = 1;
                printf("\n Username: %s\n", temp->username);
                printf("   Role: %s\n", temp->role);
                printf("   Email: %s\n", temp->email);
                char ch;
                printf("   Approve this user? (y/n): ");
                scanf(" %c", &ch);
                clearInputBuffer();
                if (ch == 'y' || ch == 'Y') {
                    temp->isApproved = 1;
                    printf("    User approved!\n");
                } else {
                    printf("    User not approved.\n");
                }
            }
            temp = temp->next;
        }
    }
    if (!found) {
        printf("\n No pending approvals.\n");
    } else {
        saveAllAccounts();
    }
}

void initializeSystem() {

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    loadAccounts();

    if (findUserByUsername("campusCoders") == NULL) {
        User *mainAdmin = createUser("campusCoders", "pbl987",
                                     "admin@geu.ac.in", "admin", 1);
        if (mainAdmin) {
            insertUser(mainAdmin);
            saveAccount(mainAdmin);
            // printf(" Default admin created: campusCoders / pbl987\n");
            // (disabled for JSON purity in API mode)
        }
    }
}

void displayMainMenu() {
    printf("1. Login\n");
    printf("2. Sign Up (Register)\n");
    printf("3. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

void registerUserTerminal() {
    char username[50], password[50], email[100], role[20];
    int roleChoice;

    printf("\n========================================\n");
    printf("      NEW USER REGISTRATION\n");
    printf("========================================\n");

    printf("Enter username: ");
    scanf("%49s", username);

    printf("Enter password: ");
    scanf("%99s", password);

    printf("Enter email (must be @geu.ac.in): ");
    scanf("%99s", email);

    printf("\nSelect your role:\n");
    printf("1. Student\n");
    printf("2. Faculty\n");
    printf("3. Admin\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &roleChoice);
    clearInputBuffer();

    switch(roleChoice) {
        case 1:
            strcpy(role, "student");
            break;
        case 2:
            strcpy(role, "faculty");
            break;
        case 3:
            strcpy(role, "admin");
            break;
        default:
            printf("\n Invalid role selection!\n");
            return;
    }

    int res = registerUser(username, password, email, role);

    if (res == 1) {
        printf("\n Registration successful!\n");
        if (strcmp(role, "student") == 0) {
            printf(" You can login now.\n");
        } else {
            printf(" Your account will be reviewed by the admin.\n");
            printf(" Admin permission request sent!\n");
        }
    } else if (res == 0) {
        printf("\n Username already exists!\n");
    } else if (res == -2) {
        printf("\n Email must be in format: yourname@geu.ac.in\n");
    } else if (res == -3) {
        printf("\n Invalid role!\n");
    } else if (res == -1) {
        printf("\n Memory allocation failed!\n");
    } else {
        printf("\n Unknown error during registration.\n");
    }
}

User* login() {
    char username[50], password[50];

    printf("\n========================================\n");
    printf("           LOGIN\n");
    printf("========================================\n");
    printf("Enter username: ");
    scanf("%49s", username);
    printf("Enter password: ");
    scanf("%99s", password);
    clearInputBuffer();

    User *u = findUser(username, password);
    if (u == NULL) {
        printf("\n Invalid username or password.\n");
        return NULL;
    }

    if (u->isApproved == 0) {
        printf("\n Account pending admin approval.\n");
        printf(" Please wait for admin to review your account.\n");
        return NULL;
    }

    printf("\n Login successful!\n");
    printf(" Welcome, %s (%s)\n", u->username, u->role);
    return u;
}

void cleanupSystem() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        User *temp = hashTable[i];
        while (temp != NULL) {
            User *toFree = temp;
            temp = temp->next;
            free(toFree);
        }
        hashTable[i] = NULL;
    }
}
