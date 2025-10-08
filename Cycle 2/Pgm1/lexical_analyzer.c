// lexical_analyzer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if a given lexeme is a keyword
int isKeyword(char buffer[]) {
    char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default",
                             "do", "double", "else", "enum", "extern", "float", "for", "goto",
                             "if", "int", "long", "register", "return", "short", "signed",
                             "sizeof", "static", "struct", "switch", "typedef", "union",
                             "unsigned", "void", "volatile", "while"};
    int i, flag = 0;
    for (i = 0; i < 32; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            flag = 1;
            break;
        }
    }
    return flag;
}

int main() {
    char ch, buffer[15], operators[] = "+-*/%=><";
    FILE *fp;
    int i, j = 0;

    // Open the input file
    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error while opening the file\n");
        exit(0);
    }

    printf("Tokens are:\n------------------\n");

    // Loop to read the file character by character
    while ((ch = fgetc(fp)) != EOF) {

        // 1. IGNORE WHITESPACE AND NEWLINES
        // If space, tab, or newline is encountered, just continue to the next iteration.
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue;
        }

        // 2. RECOGNIZE OPERATORS
        for (i = 0; i < strlen(operators); ++i) {
            if (ch == operators[i]) {
                printf("'%c' -> OPERATOR\n", ch);
            }
        }

        // 3. RECOGNIZE IDENTIFIERS AND KEYWORDS
        if (isalpha(ch)) {
            buffer[j++] = ch;
            // Read subsequent alphanumeric characters
            while (isalnum(ch = fgetc(fp))) {
                buffer[j++] = ch;
            }
            buffer[j] = '\0'; // Null-terminate the string
            j = 0;
            // Put the last read character (which is not alphanumeric) back
            ungetc(ch, fp);

            if (isKeyword(buffer) == 1) {
                printf("'%s' -> KEYWORD\n", buffer);
            } else {
                printf("'%s' -> IDENTIFIER\n", buffer);
            }
        }

        // 4. RECOGNIZE NUMBERS
        else if (isdigit(ch)) {
            buffer[j++] = ch;
            // Read subsequent digits
            while (isdigit(ch = fgetc(fp))) {
                buffer[j++] = ch;
            }
            buffer[j] = '\0'; // Null-terminate the string
            j = 0;
            // Put the last read character (which is not a digit) back
            ungetc(ch, fp);
            printf("'%s' -> NUMBER\n", buffer);
        }

        // 5. RECOGNIZE SPECIAL SYMBOLS
        else if (ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')') {
            printf("'%c' -> SPECIAL SYMBOL\n", ch);
        }
    }

    fclose(fp);
    return 0;
}