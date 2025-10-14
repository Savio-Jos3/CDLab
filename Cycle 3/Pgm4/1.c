#include <stdio.h>
#include <string.h>

// Globals
char input[100];
char stack[100];
int input_ptr = 0;
int stack_top = -1;

// Function to print the current state
void print_state() {
    // Print stack
    for (int i = 0; i <= stack_top; i++) {
        printf("%c", stack[i]);
    }
    printf("\t\t");
    // Print input
    for (int i = input_ptr; input[i] != '\0'; i++) {
        printf("%c", input[i]);
    }
    printf("\t\t");
}

// The main shift-reduce parsing function
void sr_parse() {
    // Add dollar to end of stack and input
    stack[++stack_top] = '$';
    strcat(input, "$");

    printf("STACK\t\tINPUT\t\tACTION\n");
    printf("-----\t\t-----\t\t------\n");

    while (1) {
        print_state();
        
        // --- ACCEPT Condition ---
        if (stack[stack_top] == 'E' && stack[stack_top - 1] == '$' && input[input_ptr] == '$') {
            printf("ACCEPT\n");
            break;
        }

        // --- REDUCE Operations ---
        // Check for handles on top of the stack
        if (stack[stack_top] == 'i') {
            stack[stack_top] = 'F';
            printf("REDUCE F->i\n");
        } else if (stack[stack_top] == ')' && stack[stack_top - 1] == 'E' && stack[stack_top - 2] == '(') {
            stack_top -= 2;
            stack[stack_top] = 'F';
            printf("REDUCE F->(E)\n");
        } else if (stack[stack_top] == 'F') {
            // Check if it's part of T*F or just F
            if (stack_top > 1 && stack[stack_top-1] == '*' && stack[stack_top-2] == 'T') {
                 stack_top -= 2;
                 stack[stack_top] = 'T';
                 printf("REDUCE T->T*F\n");
            } else {
                 stack[stack_top] = 'T';
                 printf("REDUCE T->F\n");
            }
        } else if (stack[stack_top] == 'T') {
             // Check if it's part of E+T or just T
            if (stack_top > 1 && stack[stack_top-1] == '+' && stack[stack_top-2] == 'E') {
                 stack_top -= 2;
                 stack[stack_top] = 'E';
                 printf("REDUCE E->E+T\n");
            } else {
                 stack[stack_top] = 'E';
                 printf("REDUCE E->T\n");
            }
        }
        // --- SHIFT Operation ---
        else {
            printf("SHIFT\n");
            stack[++stack_top] = input[input_ptr++];
        }

        // --- ERROR Condition ---
        if (input[input_ptr] == '\0' && (stack[stack_top] != 'E' || stack[stack_top-1] != '$')) {
             if (stack[stack_top] == '$' && stack_top == 0) continue; // Allow one last check
             print_state();
             printf("ERROR\n");
             break;
        }
    }
}

int main() {
    printf("Enter the input string (e.g., i*i+i): ");
    scanf("%s", input);
    sr_parse();
    return 0;
}
