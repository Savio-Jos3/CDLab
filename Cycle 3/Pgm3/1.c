#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Globals
char input[100];
int i = 0; // Input pointer
int error = 0;

// Function prototypes for each non-terminal
void E();
void E_prime();
void T();
void T_prime();
void F();

// Function to match a terminal and advance the pointer
void match(char terminal) {
    if (input[i] == terminal) {
        i++;
    } else {
        error = 1;
    }
}

// Grammar: F -> (E) | i
void F() {
    if (input[i] == '(') {
        printf("F -> (E)\n");
        match('(');
        E();
        match(')');
    } else if (isalnum(input[i])) { // In a real scenario, this would check for 'id'
        printf("F -> i\n");
        match(input[i]); // Consumes the identifier
    } else {
        error = 1;
    }
}

// Grammar: T' -> *FT' | ε
void T_prime() {
    if (input[i] == '*') {
        printf("T' -> *FT'\n");
        match('*');
        F();
        T_prime();
    } else {
        printf("T' -> ε\n");
    }
}

// Grammar: T -> FT'
void T() {
    printf("T -> FT'\n");
    F();
    T_prime();
}

// Grammar: E' -> +TE' | ε
void E_prime() {
    if (input[i] == '+') {
        printf("E' -> +TE'\n");
        match('+');
        T();
        E_prime();
    } else {
        printf("E' -> ε\n");
    }
}

// Grammar: E -> TE'
void E() {
    printf("E -> TE'\n");
    T();
    E_prime();
}

int main() {
    printf("Enter an arithmetic expression: ");
    scanf("%s", input);

    printf("\n--- Parsing Steps ---\n");
    // Start parsing from the start symbol E
    E();

    // Check for success
    // Parsing is successful if there was no error and the entire string was consumed
    if (strlen(input) == i && error == 0) {
        printf("\nString is successfully parsed.\n");
    } else {
        printf("\nError in parsing the string.\n");
    }

    return 0;
}

