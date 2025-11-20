// ============================================
// FILE: community_api.c
// CLI wrapper for community module (for Node)
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "community.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: community_api <command>\n");
        return 1;
    }

    initializeCommunity();

    if (strcmp(argv[1], "list") == 0) {
        // Print all posts as JSON
        printAllPostsAsJson();
    }
    else if (strcmp(argv[1], "addPost") == 0) {
        // stdin: title|author|content
        char line[800];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupCommunity();
            return 1;
        }

        char* title   = strtok(line, "|");
        char* author  = strtok(NULL, "|");
        char* content = strtok(NULL, "|");

        if (!title || !author || !content) {
            fprintf(stderr, "Invalid input format.\n");
            cleanupCommunity();
            return 1;
        }

        content[strcspn(content, "\n")] = 0;
        addPostFromData(title, author, content);
        printf("OK\n");
    }
    else if (strcmp(argv[1], "addComment") == 0) {
        // stdin: postTitle|author|text
        char line[900];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupCommunity();
            return 1;
        }

        char* postTitle = strtok(line, "|");
        char* author    = strtok(NULL, "|");
        char* text      = strtok(NULL, "|");

        if (!postTitle || !author || !text) {
            fprintf(stderr, "Invalid input format.\n");
            cleanupCommunity();
            return 1;
        }

        text[strcspn(text, "\n")] = 0;
        addCommentToPostFromData(postTitle, author, text);
        printf("OK\n");
    }
    else if (strcmp(argv[1], "getPost") == 0) {
        // stdin: title\n
        char title[100];
        if (!fgets(title, sizeof(title), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupCommunity();
            return 1;
        }
        title[strcspn(title, "\n")] = 0;
        printPostWithCommentsAsJson(title);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        cleanupCommunity();
        return 1;
    }

    cleanupCommunity();
    return 0;
}
