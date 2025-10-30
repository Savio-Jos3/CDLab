#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char input[100];
char stack[100];
// The terminals array, including parentheses
char terminals[] = {'i', '+', '*', '(', ')', '$'};
int top = 0;
int i = 0;

// The precedence table (6x6) that includes ( and )
// Rows/Cols are in order: i, +, *, (, ), $
char precedence_table[6][6] = {
    //        i    +    * (    )    $
    /* i */ {'E', '>', '>', 'E', '>', '>'},
    /* + */ {'<', '>', '<', '<', '>', '>'},
    /* * */ {'<', '>', '>', '<', '>', '>'},
    /* ( */ {'<', '<', '<', '<', '=', 'E'},
    /* ) */ {'E', '>', '>', 'E', '>', '>'},
    /* $ */ {'<', '<', '<', '<', 'E', 'A'}
};

// Function to get the index of a terminal in the array
int get_terminal_index(char c) {
    int j;
    for (j = 0; j < sizeof(terminals) / sizeof(terminals[0]); j++) {
        if (c == terminals[j]) {
            return j;
        }
    }
    return -1; // Invalid terminal
}

// The main parsing function
void parse() {
    stack[top] = '$';
    printf("STACK\t\tINPUT\t\tACTION\n");
    printf("-----\t\t-----\t\t------\n");

    // Loop until both stack and input are at '$'
    while (stack[top] != '$' || input[i] != '$') {
        printf("%s\t\t%s\t\t", stack, &input[i]);

        int stack_top_idx = get_terminal_index(stack[top]);
        int input_char_idx = get_terminal_index(input[i]);

        if (stack_top_idx == -1 || input_char_idx == -1) {
            printf("ERROR: Invalid character encountered.\n");
            exit(1);
        }

        char relation = precedence_table[stack_top_idx][input_char_idx];

        switch (relation) {
            case '<': // Shift
            case '=': // Shift (used for '(' vs ')')
                printf("SHIFT\n");
                stack[++top] = input[i++];
                stack[top + 1] = '\0';
                break;
            case '>': // Reduce
                printf("REDUCE\n");
                char last_popped;
                do {
                    last_popped = stack[top--];
                    stack[top + 1] = '\0';

                    // *** BUG FIX HERE ***
                    // We must stop if we've popped the handle
                    // and uncovered the stack bottom '$'.
                    if (stack[top] == '$') {
                        break;
                    }
                    // *** END OF FIX ***

                } while (precedence_table[get_terminal_index(stack[top])][get_terminal_index(last_popped)] != '<');
                break;
            case 'A': // Accept
                printf("ACCEPT\n");
                return;
            case 'E': // Error
            default:
                printf("ERROR: Syntax error detected.\n");
                exit(1);
        }
    }

    // Final accept condition
    if (stack[top] == '$' && input[i] == '$') {
         printf("%s\t\t%s\t\tACCEPT\n", stack, &input[i]);
    }
}

int main() {
    printf("Enter the input string (use 'i' for id): ");
    scanf("%s", input);

    // Append the end-of-input marker
    int len = strlen(input);
    input[len] = '$';
    input[len + 1] = '\0';

    parse();

    return 0;
}
