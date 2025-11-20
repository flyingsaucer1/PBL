// ============================================
// FILE: auth_api.c
// CLI wrapper for auth module (for Node)
// Commands:
//   auth_api register  (stdin: username|password|email|role)
//   auth_api login     (stdin: username|password)
// Outputs: single JSON object on stdout
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: auth_api <register|login>\n");
        return 1;
    }

    initializeSystem();

    if (strcmp(argv[1], "register") == 0) {
        char line[400];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupSystem();
            return 1;
        }

        char *username = strtok(line, "|");
        char *password = strtok(NULL, "|");
        char *email    = strtok(NULL, "|");
        char *role     = strtok(NULL, "|");

        if (!username || !password || !email || !role) {
            fprintf(stderr, "Invalid input format.\n");
            cleanupSystem();
            return 1;
        }

        // Trim newline from role/email if present
        username[strcspn(username, "\r\n")] = 0;
        password[strcspn(password, "\r\n")] = 0;
        email[strcspn(email, "\r\n")]       = 0;
        role[strcspn(role, "\r\n")]         = 0;

        int rc = registerUser(username, password, email, role);

        const char* msg = "Unknown error.";
        int approved = 0;
        int success = 0;

        if (rc == 1) {
            success = 1;
            if (strcmp(role, "student") == 0) {
                approved = 1;
                msg = "Registration successful. You can now login.";
            } else {
                approved = 0;
                msg = "Registration successful. Your account is pending admin approval.";
            }
        } else if (rc == 0) {
            success = 0;
            msg = "Username already exists.";
        } else if (rc == -2) {
            success = 0;
            msg = "Email must be in format: yourname@geu.ac.in";
        } else if (rc == -3) {
            success = 0;
            msg = "Invalid role.";
        } else if (rc == -1) {
            success = 0;
            msg = "Internal error: memory allocation failed.";
        }

        printf("{\"success\":%s,\"code\":%d,\"message\":\"%s\",\"role\":\"%s\",\"approved\":%d}\n",
               success ? "true" : "false",
               rc,
               msg,
               role,
               approved);
    }
    else if (strcmp(argv[1], "login") == 0) {
        char line[200];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "No input received.\n");
            cleanupSystem();
            return 1;
        }

        char *username = strtok(line, "|");
        char *password = strtok(NULL, "|");

        if (!username || !password) {
            fprintf(stderr, "Invalid input format.\n");
            cleanupSystem();
            return 1;
        }

        username[strcspn(username, "\r\n")] = 0;
        password[strcspn(password, "\r\n")] = 0;

        int rc = loginUser(username, password);

        const char* msg = "";
        int success = 0;
        char role[20] = "";
        char email[100] = "";
        int approved = 0;

        if (rc == 1) {
            success = 1;
            msg = "Login successful.";
            getUserInfo(username, role, email, &approved);
        } else if (rc == 0) {
            success = 0;
            msg = "Invalid username or password.";
        } else if (rc == -1) {
            success = 0;
            msg = "Account pending admin approval.";
        } else {
            success = 0;
            msg = "Unknown error.";
        }

        printf("{\"success\":%s,\"code\":%d,\"message\":\"%s\",\"role\":\"%s\",\"email\":\"%s\",\"approved\":%d}\n",
               success ? "true" : "false",
               rc,
               msg,
               role,
               email,
               approved);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        cleanupSystem();
        return 1;
    }

    cleanupSystem();
    return 0;
}
