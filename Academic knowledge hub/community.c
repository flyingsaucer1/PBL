#include "community.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void loadPosts();
void saveAllPosts();
void saveAllComments();

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
    saveAllPosts();
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
    saveAllComments();
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
void initializeCommunity() {
    loadPosts();
}

void cleanupCommunity() {
    Post* p = postListHead;
    while (p) {
        Comment* c = p->comments;
        while (c) {
            Comment* toFree = c;
            c = c->next;
            free(toFree);
        }
        Post* toFree = p;
        p = p->next;
        free(toFree);
    }
    postListHead = NULL;
}

void loadPosts() {
    FILE* fp = fopen("posts.txt", "r");
    if (!fp) {
        return;
    }

    char line[600];
    while (fgets(line, sizeof(line), fp)) {
        Post* newPost = (Post*)malloc(sizeof(Post));
        if (!newPost) continue;

        char* token;
        char lineCopy[600];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(newPost->title, token, 99);
        
        token = strtok(NULL, "|");
        if (token) strncpy(newPost->author, token, 49);
        
        token = strtok(NULL, "|");
        if (token) strncpy(newPost->date, token, 19);
        
        token = strtok(NULL, "|");
        if (token) {
            strncpy(newPost->content, token, MAX_POST - 1);
            newPost->content[strcspn(newPost->content, "\n")] = 0;
        }

        newPost->comments = NULL;
        newPost->next = postListHead;
        postListHead = newPost;
    }
    fclose(fp);

    // Load comments
    fp = fopen("comments.txt", "r");
    if (!fp) return;

    while (fgets(line, sizeof(line), fp)) {
        char postTitle[100], author[50], text[MAX_COMMENT];
        char* token;
        char lineCopy[600];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, "|");
        if (token) strncpy(postTitle, token, 99);
        
        token = strtok(NULL, "|");
        if (token) strncpy(author, token, 49);
        
        token = strtok(NULL, "|");
        if (token) {
            strncpy(text, token, MAX_COMMENT - 1);
            text[strcspn(text, "\n")] = 0;
        }

        Post* p = postListHead;
        while (p) {
            if (strcmp(p->title, postTitle) == 0) {
                Comment* newComment = (Comment*)malloc(sizeof(Comment));
                if (newComment) {
                    strcpy(newComment->author, author);
                    strcpy(newComment->text, text);
                    newComment->next = NULL;

                    if (!p->comments) {
                        p->comments = newComment;
                    } else {
                        Comment* last = p->comments;
                        while (last->next) last = last->next;
                        last->next = newComment;
                    }
                }
                break;
            }
            p = p->next;
        }
    }
    fclose(fp);
    printf(" Loaded community posts.\n");
}

void saveAllPosts() {
    FILE* fp = fopen("posts.txt", "w");
    if (!fp) {
        printf(" Error: Cannot save posts!\n");
        return;
    }

    Post* p = postListHead;
    while (p) {
        fprintf(fp, "%s|%s|%s|%s\n", 
                p->title, p->author, p->date, p->content);
        p = p->next;
    }
    fflush(fp);
    fclose(fp);
}

void saveAllComments() {
    FILE* fp = fopen("comments.txt", "w");
    if (!fp) {
        printf(" Error: Cannot save comments!\n");
        return;
    }

    Post* p = postListHead;
    while (p) {
        Comment* c = p->comments;
        while (c) {
            fprintf(fp, "%s|%s|%s\n", 
                    p->title, c->author, c->text);
            c = c->next;
        }
        p = p->next;
    }
    fflush(fp);
    fclose(fp);
}