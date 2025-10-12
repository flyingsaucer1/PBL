#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_SUBJECT 100
#define MAX_TITLE 100
#define MAX_CONTENT 500
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_EMAIL 100
#define MAX_ROLE 20
#define MAX_POST 500
#define MAX_COMMENT 200
#define TABLE_SIZE 100

typedef struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_EMAIL];
    char role[MAX_ROLE];  
    int isApproved;       
    struct User* next;    
} User;

typedef struct Note {
    char title[MAX_TITLE];
    char subject[MAX_SUBJECT];
    char content[MAX_CONTENT];
    struct Note* next;
} Note;

typedef struct Quiz {
    char title[MAX_TITLE];
    char subject[MAX_SUBJECT];
    struct Quiz* next;
} Quiz;

typedef struct Comment {
    char author[MAX_USERNAME];
    char text[MAX_COMMENT];
    struct Comment* next;
} Comment;

typedef struct Post {
    char title[MAX_TITLE];
    char content[MAX_POST];
    char author[MAX_USERNAME];
    char date[20];
    Comment* comments;
    struct Post* next;
} Post;

#endif
