// ============================================
// FILE: community.h
// Community Module Header
// ============================================

#ifndef COMMUNITY_H
#define COMMUNITY_H

#include "structures.h"

typedef struct Comment {
    char author[50];
    char text[MAX_COMMENT];
    struct Comment* next;
} Comment;

typedef struct Post {
    char title[100];
    char author[50];
    char date[20];
    char content[MAX_POST];
    Comment* comments;
    struct Post* next;
} Post;

void createPost(char* username);
void viewAllPosts();
void addComment(char* username);
void viewPostWithComments();
void initializeCommunity();
void cleanupCommunity();

#endif // COMMUNITY_H