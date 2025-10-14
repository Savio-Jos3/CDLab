#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Globals
int num_productions;
char productions[20][20];
char first_sets[26][100]; // To store FIRST sets for A-Z
char follow_sets[26][100]; // To store FOLLOW sets for A-Z
int visited_first[26] = {0};

// Function to add a character to a set, avoiding duplicates
void add_to_set(char* set, char c) {
    if (strchr(set, c) == NULL) { // Check if character is not already in the set
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// Function to find the FIRST set of a non-terminal
void find_first(char non_terminal) {
    if (visited_first[non_terminal - 'A']) {
        return;
    }

    int i, k;
    for (i = 0; i < num_productions; i++) {
        if (productions[i][0] == non_terminal) {
            if (productions[i][2] == '#') { // Epsilon production (e.g., A=#)
                add_to_set(first_sets[non_terminal - 'A'], '#');
            } else if (islower(productions[i][2]) || !isalpha(productions[i][2])) { // Terminal
                add_to_set(first_sets[non_terminal - 'A'], productions[i][2]);
            } else { // Non-terminal
                // Loop through symbols in the production's RHS
                for (k = 2; productions[i][k] != '\0'; k++) {
                    char next_symbol = productions[i][k];
                    if (!isupper(next_symbol)) { // It's a terminal
                        add_to_set(first_sets[non_terminal - 'A'], next_symbol);
                        break; // Stop after first terminal
                    }

                    // It's a non-terminal
                    find_first(next_symbol);
                    // Add its FIRST set to current non-terminal's FIRST set
                    int j = 0;
                    int has_epsilon = 0;
                    while (first_sets[next_symbol - 'A'][j] != '\0') {
                        if (first_sets[next_symbol - 'A'][j] != '#') {
                            add_to_set(first_sets[non_terminal - 'A'], first_sets[next_symbol - 'A'][j]);
                        } else {
                            has_epsilon = 1;
                        }
                        j++;
                    }

                    // If epsilon is not in the FIRST set of the next symbol, break
                    if (!has_epsilon) {
                        break;
                    }
                    
                    // If we are at the end and all previous symbols had epsilon
                    if (productions[i][k + 1] == '\0') {
                        add_to_set(first_sets[non_terminal - 'A'], '#');
                    }
                }
            }
        }
    }
    visited_first[non_terminal - 'A'] = 1;
}

int main() {
    int i, j;
    char non_terminals[26] = {0};

    printf("Enter the number of productions: ");
    scanf("%d", &num_productions);

    // Consume the trailing newline character from the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter the productions (e.g., E=T+E, use '#' for epsilon):\n");
    for (i = 0; i < num_productions; i++) {
        printf("[%d]: ", i + 1);
        scanf("%19s", productions[i]); // Use width specifier to prevent buffer overflow
        add_to_set(non_terminals, productions[i][0]);
    }

    // Calculate FIRST sets
    for (i = 0; i < strlen(non_terminals); i++) {
        find_first(non_terminals[i]);
    }

    // --- Calculate FOLLOW sets iteratively ---
    
    // Rule 1: Add $ to FOLLOW of the start symbol
    add_to_set(follow_sets[productions[0][0] - 'A'], '$');

    int changed = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < num_productions; i++) { // For each production A -> beta
            char A = productions[i][0];
            for (j = 2; productions[i][j] != '\0'; j++) {
                char B = productions[i][j];
                if (isupper(B)) { // If it's a non-terminal
                    int old_len = strlen(follow_sets[B - 'A']);
                    
                    // Find FIRST of the rest of the string (gamma in A -> aBg)
                    int k = j + 1;
                    int has_epsilon = 1; // Assume epsilon until proven otherwise
                    
                    if (productions[i][k] != '\0') { // If B is not the last symbol
                        has_epsilon = 0;
                        // Loop through the rest of the production string (gamma)
                        for(; productions[i][k] != '\0'; k++) {
                           char next_sym = productions[i][k];
                           has_epsilon = 0;
                           if(isupper(next_sym)) { // Non-terminal
                               int l = 0;
                               while(first_sets[next_sym - 'A'][l] != '\0') {
                                   if(first_sets[next_sym - 'A'][l] != '#') {
                                       add_to_set(follow_sets[B - 'A'], first_sets[next_sym - 'A'][l]);
                                   } else {
                                       has_epsilon = 1;
                                   }
                                   l++;
                               }
                           } else { // Terminal
                               add_to_set(follow_sets[B - 'A'], next_sym);
                               break; // Terminal stops the process
                           }
                           if(!has_epsilon) break;
                        }
                    }

                    // Rule 3: If it's the end of production or FIRST(gamma) contains epsilon
                    if (has_epsilon) {
                        int l = 0;
                        while(follow_sets[A - 'A'][l] != '\0') {
                            add_to_set(follow_sets[B - 'A'], follow_sets[A - 'A'][l]);
                            l++;
                        }
                    }
                    
                    if (strlen(follow_sets[B - 'A']) > old_len) {
                        changed = 1;
                    }
                }
            }
        }
    }

    printf("\n--- FIRST Sets ---\n");
    for (i = 0; i < strlen(non_terminals); i++) {
        printf("FIRST(%c) = { %s }\n", non_terminals[i], first_sets[non_terminals[i] - 'A']);
    }

    printf("\n--- FOLLOW Sets ---\n");
    for (i = 0; i < strlen(non_terminals); i++) {
        printf("FOLLOW(%c) = { %s }\n", non_terminals[i], follow_sets[non_terminals[i] - 'A']);
    }

    return 0;
}

