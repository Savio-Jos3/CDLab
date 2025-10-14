#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Globals
char input[100];
int pos = 0;
int temp_count = 0;

// Function prototypes
char* E();
char* E_prime(char*);
char* T();
char* T_prime(char*);
char* F();

// Helper to generate new temporary variable names
char* new_temp() {
    char* temp = (char*)malloc(4 * sizeof(char));
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

// Factor: F -> (E) | id
char* F() {
    char* result;
    if (input[pos] == '(') {
        pos++;
        result = E();
        if (input[pos] == ')') {
            pos++;
            return result;
        }
    } else if (isalnum(input[pos])) {
        result = (char*)malloc(2 * sizeof(char));
        result[0] = input[pos];
        result[1] = '\0';
        pos++;
        return result;
    }
    printf("Error in F\n");
    exit(1);
}

// T' -> *FT' | /FT' | ε
char* T_prime(char* inherited_result) {
    if (input[pos] == '*' || input[pos] == '/') {
        char op = input[pos];
        pos++;
        char* factor_result = F();
        char* new_result = new_temp();
        printf("%s = %s %c %s\n", new_result, inherited_result, op, factor_result);
        return T_prime(new_result);
    }
    // Epsilon case
    return inherited_result;
}

// Term: T -> FT'
char* T() {
    char* factor_result = F();
    return T_prime(factor_result);
}

// E' -> +TE' | -TE' | ε
char* E_prime(char* inherited_result) {
    if (input[pos] == '+' || input[pos] == '-') {
        char op = input[pos];
        pos++;
        char* term_result = T();
        char* new_result = new_temp();
        printf("%s = %s %c %s\n", new_result, inherited_result, op, term_result);
        return E_prime(new_result);
    }
    // Epsilon case
    return inherited_result;
}

// Expression: E -> TE'
char* E() {
    char* term_result = T();
    return E_prime(term_result);
}

int main() {
    printf("Enter an arithmetic expression: ");
    scanf("%s", input);
    
    printf("\n--- Three-Address Code ---\n");
    char* final_result = E();
    // In a real compiler, you might assign this to a variable.
    // For this example, we just show the generated code.

    if (input[pos] != '\0') {
         printf("\nError: Invalid expression or extra characters.\n");
    }

    return 0;
}
