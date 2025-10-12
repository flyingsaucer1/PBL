#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10


typedef struct User {
    char username[50];
    char password[50];
    char role[20];       // student, faculty, admin
    int isApproved;      // 1 = approved, 0 = pending
    struct User *next;  
} User;

// Hash table (array of linked list heads)
User *hashTable[SIZE];

//  hash
int hash(char *username) {
    int sum = 0;
    for (int i = 0; username[i]; i++)
        sum += username[i];
    return sum % SIZE;
}

// creat a user
User* create(char username[], char password[], char role[], int isApproved) {
    User *newUser = (User*) malloc(sizeof(User));
    strcpy( newUser ->username, username);
    strcpy(newUser->password, password);
    strcpy(newUser->role, role);
    newUser->isApproved = isApproved;
    newUser->next = NULL;
    return newUser;
}

//  inser user
void insert(User *user) {
    int index = hash(user->username);
    user->next = hashTable[index];
    hashTable[index] = user;
}

// Search user for next time login 
User* find(char username[], char password[]) {
    int index = hash(username);
    User *temp = hashTable[index];
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// Register function for those who use first time 
void register(char role[]) {
    char username[50], password[50];
    printf("\nEnter %s username: ", role);
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    int approved = (strcmp(role, "student") == 0) ? 1 : 0;  // students auto-approved
    User *newUser = create(username, password, role, approved);
    insert(newUser);
    printf("\nRegistration successful! ");
    if (approved)
        printf("You can login now.\n");
    else
        printf("Wait for admin approval.\n");
}

//  admin approval for faculty 
void approve_accounts() {
    printf("\n Pending Accounts \n");
    for (int i = 0; i < SIZE; i++) {
        User *temp = hashTable[i];
        while (temp != NULL) {
            if (temp->isApproved == 0 && (strcmp(temp->role, "faculty") == 0 || strcmp(temp->role, "admin") == 0)) {
                printf("Pending: %s (%s)\n", temp->username, temp->role);
                char ch;
                printf("Approve this user? (y/n): ");
                scanf(" %c", &ch);
                if (ch == 'y' || ch == 'Y')
                    temp->isApproved = 1;
            }
            temp = temp->next;
        }
    }
    printf("\nAll pending approvals processed.\n");
}

// MENUS 
void student_menu() {
    printf("\n Welcome, Student!\n");
    printf("You can access your study materials.\n");
}

void faculty_menu() {
    printf("\n Welcome, Faculty!\n");
    printf("You can upload your study materials.\n");
}

void admin_menu(User *currentUser) {
    printf("\n Welcome, Admin %s!\n", currentUser->username);
    printf("1. Approve Accounts\n");
    printf("2. Logout\n");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
        approve_accounts();
}

// LOGIN FUNCTION 
void login() {
    char username[50], password[50];
    printf("\nEnter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    User *u = find(username, password);
    if (u == NULL) {
        printf("\n Invalid username or password.\n");
        return;
    }

    if (u->isApproved == 0) {
        printf("\n⚠ Account pending admin approval.\n");
        return;
    }

    if (strcmp(u->role, "student") == 0)
        student_menu();
    else if (strcmp(u->role, "faculty") == 0)
        faculty_menu();
    else if (strcmp(u->role, "admin") == 0)
        admin_menu(u);
}

// INITIALIZE DEFAULT ADMIN 
void initializeSystem() {
    User *mainAdmin = create("campus_coders", "GEU", "admin", 1);
    insert(mainAdmin);
}

// MAIN FUNCTION 
int main() {
    initializeSystem();

    int choice;
    while (1) {
        printf("\n===== Educational Management System =====\n");
        printf("1. Register Student\n");
        printf("2. Register Faculty\n");
        printf("3. Register Admin\n");
        printf("4. Login\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: register("student"); break;
            case 2: register("faculty"); break;
            case 3: register("admin"); break;
            case 4: login(); break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}
//ye h authentication h run kar ke dekh lo sare case mai chal rha h vese to lekin fir bhi ek bar
