#include "community.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Post* postListHead = NULL;

void getDate(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void createPost(char* username) {
    Post* newPost = (Post*)malloc(sizeof(Post));
    if(newPost == NULL) {
        printf("\n Memory error!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      CREATE NEW POST\n");
    printf("========================================\n");
    strcpy(newPost->author, username);
    printf("Enter Post Title: ");
    fgets(newPost->title, 100, stdin);
    newPost->title[strcspn(newPost->title, "\n")] = 0;  
    printf("Enter Post Content: ");
    fgets(newPost->content, MAX_POST, stdin);
    newPost->content[strcspn(newPost->content, "\n")] = 0;  
    getDate(newPost->date);
    newPost->comments = NULL;
    newPost->next = postListHead;
    postListHead = newPost;
    printf("\n Post created successfully!\n");
}
void viewAllPosts() {
    if(postListHead == NULL) {
        printf("\n  No posts available! Be the first to post.\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      COMMUNITY FEED\n");
    printf("========================================\n");
    
    Post* current = postListHead;
    int postNumber = 1;
    while(current != NULL) {
        printf("\n%d. Title: %s\n", postNumber, current->title);
        printf("   Author: %s\n", current->author);
        printf("   Date: %s\n", current->date);
        printf("   Content: %s\n", current->content);
        int commentCount = 0;
        Comment* c = current->comments;
        while(c != NULL) {
            commentCount++;
            c = c->next;
        }
        printf("    Comments: %d\n", commentCount);
        printf("----------------------------------------\n");
        
        current = current->next;
        postNumber++;
    }
}
void addComment(char* username) {
    if(postListHead == NULL) {
        printf("\n  No posts available to comment on!\n");
        return;
    }
    viewAllPosts();
    char postTitle[100];
    printf("\nEnter post title to comment on: ");
    fgets(postTitle, 100, stdin);
    postTitle[strcspn(postTitle, "\n")] = 0;  
    Post* current = postListHead;
    while(current != NULL) {
        if(strcasecmp(current->title, postTitle) == 0) {
            Comment* newComment = (Comment*)malloc(sizeof(Comment));
            if(newComment == NULL) {
                printf("\n Memory error!\n");
                return;
            }
            strcpy(newComment->author, username);
            printf("Enter your comment: ");
            fgets(newComment->text, MAX_COMMENT, stdin);
            newComment->text[strcspn(newComment->text, "\n")] = 0;  
            newComment->next = NULL;
            if(current->comments == NULL) {
                current->comments = newComment;
            } else {
                Comment* lastComment = current->comments;
                while(lastComment->next != NULL) {
                    lastComment = lastComment->next;
                }
                lastComment->next = newComment;
            }
            
            printf("\n Comment added successfully!\n");
            return;
        }
        current = current->next;
    }
    
    printf("\n Post not found! Check the title and try again.\n");
}
void viewPostWithComments() {
    if(postListHead == NULL) {
        printf("\n  No posts available!\n");
        return;
    }
    viewAllPosts();
    char postTitle[100];
    printf("\nEnter post title to view with comments: ");
    fgets(postTitle, 100, stdin);
    postTitle[strcspn(postTitle, "\n")] = 0;  
    Post* current = postListHead;
    while(current != NULL) {
        if(strcasecmp(current->title, postTitle) == 0) {
            printf("\n========================================\n");
            printf(" Title: %s\n", current->title);
            printf(" Author: %s\n", current->author);
            printf(" Date: %s\n", current->date);
            printf("========================================\n");
            printf(" Content:\n%s\n", current->content);
            printf("========================================\n");
            if(current->comments == NULL) {
                printf("\n  No comments yet. Be the first to comment!\n");
            } else {
                printf("\n COMMENTS:\n");
                printf("----------------------------------------\n");
                
                Comment* c = current->comments;
                int commentNumber = 1;
                
                while(c != NULL) {
                    printf("%d. %s wrote:\n", commentNumber, c->author);
                    printf("   %s\n", c->text);
                    printf("----------------------------------------\n");
                    c = c->next;
                    commentNumber++;
                }
            }
            printf("========================================\n");
            return;
        }
        current = current->next;
    }
    
    printf("\n Post not found! Check the title and try again.\n");
}