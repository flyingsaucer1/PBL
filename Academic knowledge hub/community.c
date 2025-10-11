#include "community.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global list to store all posts
Post* postListHead = NULL;

// Helper function to get current date
void getDate(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

// Function 1: Create a new post
void createPost(char* username) {
    // Allocate memory for new post
    Post* newPost = (Post*)malloc(sizeof(Post));
    if(newPost == NULL) {
        printf("\n❌ Memory error!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      CREATE NEW POST\n");
    printf("========================================\n");
    
    // Set author
    strcpy(newPost->author, username);
    
    // Get title from user
    printf("Enter Post Title: ");
    fgets(newPost->title, 100, stdin);
    newPost->title[strcspn(newPost->title, "\n")] = 0;  // Remove newline
    
    // Get content from user
    printf("Enter Post Content: ");
    fgets(newPost->content, MAX_POST, stdin);
    newPost->content[strcspn(newPost->content, "\n")] = 0;  // Remove newline
    
    // Set date
    getDate(newPost->date);
    
    // Initialize comments as empty
    newPost->comments = NULL;
    
    // Add to beginning of list (Stack - newest first)
    newPost->next = postListHead;
    postListHead = newPost;
    
    printf("\n✅ Post created successfully!\n");
}

// Function 2: View all posts
void viewAllPosts() {
    // Check if list is empty
    if(postListHead == NULL) {
        printf("\n⚠️  No posts available! Be the first to post.\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("      COMMUNITY FEED\n");
    printf("========================================\n");
    
    Post* current = postListHead;
    int postNumber = 1;
    
    // Loop through all posts
    while(current != NULL) {
        printf("\n%d. Title: %s\n", postNumber, current->title);
        printf("   Author: %s\n", current->author);
        printf("   Date: %s\n", current->date);
        printf("   Content: %s\n", current->content);
        
        // Count comments
        int commentCount = 0;
        Comment* c = current->comments;
        while(c != NULL) {
            commentCount++;
            c = c->next;
        }
        printf("   💬 Comments: %d\n", commentCount);
        printf("----------------------------------------\n");
        
        current = current->next;
        postNumber++;
    }
}

// Function 3: Add comment to a post
void addComment(char* username) {
    // Check if any posts exist
    if(postListHead == NULL) {
        printf("\n⚠️  No posts available to comment on!\n");
        return;
    }
    
    // Show all posts
    viewAllPosts();
    
    // Ask which post to comment on
    char postTitle[100];
    printf("\nEnter post title to comment on: ");
    fgets(postTitle, 100, stdin);
    postTitle[strcspn(postTitle, "\n")] = 0;  // Remove newline
    
    // Find the post
    Post* current = postListHead;
    while(current != NULL) {
        if(strcasecmp(current->title, postTitle) == 0) {
            // Post found! Create new comment
            Comment* newComment = (Comment*)malloc(sizeof(Comment));
            if(newComment == NULL) {
                printf("\n❌ Memory error!\n");
                return;
            }
            
            // Set comment author
            strcpy(newComment->author, username);
            
            // Get comment text
            printf("Enter your comment: ");
            fgets(newComment->text, MAX_COMMENT, stdin);
            newComment->text[strcspn(newComment->text, "\n")] = 0;  // Remove newline
            
            newComment->next = NULL;
            
            // Add comment to post
            if(current->comments == NULL) {
                // First comment
                current->comments = newComment;
            } else {
                // Add to end of comments
                Comment* lastComment = current->comments;
                while(lastComment->next != NULL) {
                    lastComment = lastComment->next;
                }
                lastComment->next = newComment;
            }
            
            printf("\n✅ Comment added successfully!\n");
            return;
        }
        current = current->next;
    }
    
    printf("\n❌ Post not found! Check the title and try again.\n");
}

// Function 4: View a specific post with all its comments
void viewPostWithComments() {
    // Check if any posts exist
    if(postListHead == NULL) {
        printf("\n⚠️  No posts available!\n");
        return;
    }
    
    // Show all posts
    viewAllPosts();
    
    // Ask which post to view
    char postTitle[100];
    printf("\nEnter post title to view with comments: ");
    fgets(postTitle, 100, stdin);
    postTitle[strcspn(postTitle, "\n")] = 0;  // Remove newline
    
    // Find the post
    Post* current = postListHead;
    while(current != NULL) {
        if(strcasecmp(current->title, postTitle) == 0) {
            // Post found! Display it
            printf("\n========================================\n");
            printf("📝 Title: %s\n", current->title);
            printf("👤 Author: %s\n", current->author);
            printf("📅 Date: %s\n", current->date);
            printf("========================================\n");
            printf("📄 Content:\n%s\n", current->content);
            printf("========================================\n");
            
            // Show all comments
            if(current->comments == NULL) {
                printf("\n⚠️  No comments yet. Be the first to comment!\n");
            } else {
                printf("\n💬 COMMENTS:\n");
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
    
    printf("\n❌ Post not found! Check the title and try again.\n");
}