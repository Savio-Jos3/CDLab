#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char input[100];
char stack[100];
char terminals[] = {'i', '+', '*', '(', ')', '$'};
int top = 0;
int i = 0;

char precedence_table[6][6] = {
    {'E', '>', '>', 'E', '>', '>'},
    {'<', '>', '<', '<', '>', '>'},
    {'<', '>', '>', '<', '>', '>'},
    {'<', '<', '<', '<', '=', 'E'},
    {'E', '>', '>', 'E', '>', '>'},
    {'<', '<', '<', '<', 'E', 'A'}
};

int get_terminal_index(char c) {
    int j;
    for (j = 0; j < sizeof(terminals) / sizeof(terminals[0]); j++) {
        if (c == terminals[j]) {
            return j;
        }
    }
    return -1;
}

void parse() {
    stack[top] = '$';
    printf("STACK\t\tINPUT\t\tACTION\n");
    printf("-----\t\t-----\t\t------\n");

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
            case '<':
            case '=':
                printf("SHIFT\n");
                stack[++top] = input[i++];
                stack[top + 1] = '\0';
                break;
            case '>':
                printf("REDUCE\n");
                char last_popped;
                do {
                    last_popped = stack[top--];
                    stack[top + 1] = '\0';
                } while (precedence_table[get_terminal_index(stack[top])][get_terminal_index(last_popped)] != '<');
                break;
            case 'A':
                printf("ACCEPT\n");
                return;
            case 'E':
            default:
                printf("ERROR: Syntax error detected.\n");
                exit(1);
        }
    }

    if (stack[top] == '$' && input[i] == '$') {
         printf("%s\t\t%s\t\tACCEPT\n", stack, &input[i]);
    }
}

int main() {
    printf("Enter the input string (use 'i' for id): ");
    scanf("%s", input);

    int len = strlen(input);
    input[len] = '$';
    input[len + 1] = '\0';

    parse();

    return 0;
}