// ============================================
// FILE: auth.c
// Enhanced Authentication Module Implementation
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"
#define DB_FILE "accounts.txt"

// Global hash table
User *hashTable[TABLE_SIZE];

// ============================================
// UTILITY FUNCTIONS
// ============================================

// Validate email format (must be @geu.ac.in)
int validateEmail(char *email) {
    char *domain = strstr(email, "@geu.ac.in");
    if (domain == NULL) return 0;
    // Check if @geu.ac.in is at the end
    if (strlen(domain) == 10) return 1; // "@geu.ac.in" is 10 chars
    return 0;
}

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================
// HASH FUNCTION
// ============================================

int hash(char *username) {
    int sum = 0;
    for (int i = 0; username[i]; i++)
        sum += username[i];
    return sum % TABLE_SIZE;
}

// ============================================
// FILE PERSISTENCE FUNCTIONS
// ============================================

void loadAccounts() {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) return;

    char line[300];
    while(fgets(line, sizeof(line), fp)) {
        User *acc = malloc(sizeof(User));
        if (!acc) continue;
        
        sscanf(line, "%49[^,],%99[^,],%99[^,],%19[^,],%d",
               acc->username, acc->password, acc->email, acc->role, &acc->isApproved);
        
        // Insert into hash table
        int index = hash(acc->username);
        acc->next = hashTable[index];
        hashTable[index] = acc;
    }
    fclose(fp);
}

void saveAccount(User *acc) {
    FILE *fp = fopen(DB_FILE, "a");
    if (!fp) return;

    fprintf(fp, "%s,%s,%s,%s,%d\n", 
            acc->username, acc->password, acc->email, acc->role, acc->isApproved);
    fclose(fp);
}

void saveAllAccounts() {
    FILE *fp = fopen(DB_FILE, "w");
    if (!fp) return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        User *temp = hashTable[i];
        while (temp != NULL) {
            fprintf(fp, "%s,%s,%s,%s,%d\n", 
                    temp->username, temp->password, temp->email, temp->role, temp->isApproved);
            temp = temp->next;
        }
    }
    fclose(fp);
}

// ============================================
// USER MANAGEMENT FUNCTIONS
// ============================================

User* createUser(char username[], char password[], char email[], char role[], int isApproved) {
    User *newUser = (User*) malloc(sizeof(User));
    if (!newUser) return NULL;
    
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    strcpy(newUser->email, email);
    strcpy(newUser->role, role);
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
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
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

// ============================================
// REGISTRATION FUNCTION (Enhanced)
// ============================================

int registerUser(char username[], char password[], char email[], char role[]) {
    // Validate email format
    if (!validateEmail(email)) {
        printf("\n❌ Email must be in format: yourname@geu.ac.in\n");
        return -2; // Invalid email format
    }

    // Check if username exists
    if (findUserByUsername(username) != NULL) {
        return 0; // Username already exists
    }

    // Determine approval status
    int approved;
    if (strcmp(role, "student") == 0) {
        approved = 1; // Students auto-approved
    } else if (strcmp(role, "faculty") == 0 || strcmp(role, "admin") == 0) {
        approved = 0; // Faculty and Admin need approval
        printf("\n⚠️  Your account will be reviewed by the admin.\n");
        printf("📧 Admin permission request sent!\n");
    } else {
        return -3; // Invalid role
    }

    User *newUser = createUser(username, password, email, role, approved);
    
    if (!newUser) return -1; // Memory error
    
    insertUser(newUser);
    saveAccount(newUser); // persist immediately
    return 1; // Success
}

// ============================================
// LOGIN FUNCTION (Enhanced)
// ============================================

int loginUser(char username[], char password[]) {
    User *u = findUser(username, password);
    if (u == NULL) return 0; // Invalid credentials
    
    if (u->isApproved == 0) return -1; // Pending approval
    
    return 1; // Success
}

void getUserInfo(char username[], char *role, char *email, int *isApproved) {
    User *u = findUserByUsername(username);
    if (u) {
        strcpy(role, u->role);
        strcpy(email, u->email);
        *isApproved = u->isApproved;
    }
}

// ============================================
// ADMIN FUNCTIONS
// ============================================

int approveUser(char username[]) {
    User *u = findUserByUsername(username);
    if (u && u->isApproved == 0) {
        u->isApproved = 1;
        saveAllAccounts(); // Update file
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
                printf("\n📋 Username: %s\n", temp->username);
                printf("   Role: %s\n", temp->role);
                printf("   Email: %s\n", temp->email);
                char ch;
                printf("   Approve this user? (y/n): ");
                scanf(" %c", &ch);
                clearInputBuffer();
                if (ch == 'y' || ch == 'Y') {
                    temp->isApproved = 1;
                    printf("   ✅ User approved!\n");
                } else {
                    printf("   ❌ User not approved.\n");
                }
            }
            temp = temp->next;
        }
    }
    if (!found) {
        printf("\n⚠️  No pending approvals.\n");
    }
    saveAllAccounts(); // Save changes
}

// ============================================
// SYSTEM INITIALIZATION
// ============================================

void initializeSystem() {
    // Clear hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
    
    // Load existing accounts
    loadAccounts();
    
    // Check if default admin exists
    if (findUserByUsername("campusCoders") == NULL) {
        User *mainAdmin = createUser("campusCoders", "pbl987", "admin@geu.ac.in", "admin", 1);
        insertUser(mainAdmin);
        saveAccount(mainAdmin);
        printf("✅ Default admin created: campusCoders / pbl987\n");
    }
}

// ============================================
// ENHANCED TERMINAL MODE FUNCTIONS
// ============================================

void displayMainMenu() {
    printf("\n========================================\n");
    printf("   SMART STUDY HUB - GEU\n");
    printf("========================================\n");
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
    scanf("%s", username);
    
    printf("Enter password: ");
    scanf("%s", password);
    
    printf("Enter email (must be @geu.ac.in): ");
    scanf("%s", email);
    
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
            printf("\n❌ Invalid role selection!\n");
            return;
    }
    
    int result = registerUser(username, password, email, role);
    
    if (result == 1) {
        printf("\n✅ Registration successful!\n");
        if (strcmp(role, "student") == 0) {
            printf("✅ You can login now.\n");
        } else {
            printf("⏳ Waiting for admin approval...\n");
            printf("📧 Check back later or contact admin.\n");
        }
    } else if (result == 0) {
        printf("\n❌ Username already exists!\n");
    } else if (result == -2) {
        printf("❌ Invalid email format!\n");
    } else {
        printf("\n❌ Registration failed!\n");
    }
}

User* login() {
    char username[50], password[50];
    
    printf("\n========================================\n");
    printf("           LOGIN\n");
    printf("========================================\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    clearInputBuffer();

    User *u = findUser(username, password);
    if (u == NULL) {
        printf("\n❌ Invalid username or password.\n");
        return NULL;
    }

    if (u->isApproved == 0) {
        printf("\n⏳ Account pending admin approval.\n");
        printf("📧 Please wait for admin to review your account.\n");
        return NULL;
    }

    printf("\n✅ Login successful!\n");
    printf("👋 Welcome, %s (%s)\n", u->username, u->role);
    return u;
}
