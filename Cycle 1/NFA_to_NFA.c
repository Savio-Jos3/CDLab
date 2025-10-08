#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 50
#define MAX_SYMBOLS 10

// --- Global Data Structures ---

// For the input NFA with epsilon transitions
int nfa_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int nfa_counts[MAX_STATES][MAX_SYMBOLS];
int epsilon_transitions[MAX_STATES][MAX_STATES];
int epsilon_counts[MAX_STATES];

// For the output NFA without epsilon transitions
int final_nfa_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int final_nfa_counts[MAX_STATES][MAX_SYMBOLS];

// For storing epsilon closures for each state
int all_closures[MAX_STATES][MAX_STATES];
int all_closures_counts[MAX_STATES];

// General utility sets
int set_a[MAX_STATES], count_a;
int set_b[MAX_STATES], count_b;

// --- Set Utility Functions ---

// Helper to check if an item is in a set
int isInSet(int item, int *set, int count) {
    for (int i = 0; i < count; i++) {
        if (set[i] == item) return 1;
    }
    return 0;
}

// Helper to add an item to a set if not present
void addToSet(int item, int *set, int *count) {
    if (!isInSet(item, set, *count)) {
        set[(*count)++] = item;
    }
}

// --- Epsilon Closure Calculation (adapted from previous version) ---

void findEpsilonClosure(int start_state, int *closure_set, int *closure_count) {
    int stack[MAX_STATES];
    int top = -1;
    *closure_count = 0;

    addToSet(start_state, closure_set, closure_count);
    stack[++top] = start_state;

    while (top != -1) {
        int u = stack[top--];
        for (int i = 0; i < epsilon_counts[u]; i++) {
            int v = epsilon_transitions[u][i];
            if (!isInSet(v, closure_set, *closure_count)) {
                addToSet(v, closure_set, closure_count);
                stack[++top] = v;
            }
        }
    }
}

// --- Main Conversion Logic ---

int main() {
    int num_states, num_symbols, num_final_states;
    int initial_final_states[MAX_STATES];
    int final_nfa_final_states[MAX_STATES];
    int final_nfa_final_count = 0;

    // 1. Get Input for NFA with Epsilon
    printf("Enter the number of states: ");
    scanf("%d", &num_states);

    printf("Enter the number of symbols (excluding epsilon): ");
    scanf("%d", &num_symbols);

    printf("\nEnter transitions (from_state symbol to_state). Use -1 for no more transitions from a state.\n");
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_symbols; j++) {
            printf("Transitions from state q%d on symbol %d: ", i, j);
            int to_state;
            while (1) {
                scanf("%d", &to_state);
                if (to_state == -1) break;
                nfa_transitions[i][j][nfa_counts[i][j]++] = to_state;
            }
        }
    }

    printf("\nEnter epsilon transitions (from_state to_state). Use -1 for no more.\n");
    for (int i = 0; i < num_states; i++) {
        printf("Epsilon transitions from state q%d: ", i);
        int to_state;
        while(1) {
            scanf("%d", &to_state);
            if (to_state == -1) break;
            epsilon_transitions[i][epsilon_counts[i]++] = to_state;
        }
    }

    printf("\nEnter number of final states: ");
    scanf("%d", &num_final_states);
    printf("Enter the final states: ");
    for (int i = 0; i < num_final_states; i++) {
        scanf("%d", &initial_final_states[i]);
    }

    // 2. Pre-calculate all epsilon closures
    for (int i = 0; i < num_states; i++) {
        findEpsilonClosure(i, all_closures[i], &all_closures_counts[i]);
    }

    // 3. Calculate new transitions for the final NFA
    for (int i = 0; i < num_states; i++) { // For each state qi
        for (int j = 0; j < num_symbols; j++) { // For each symbol j
            
            // Logic: δ'(qi, a) = U ε-closure(r) for r in δ(p, a) where p in ε-closure(qi)
            count_a = 0; // Set of states reachable from ε-closure(qi) on symbol j
            
            // Find all states p in ε-closure(qi)
            for (int k = 0; k < all_closures_counts[i]; k++) {
                int p = all_closures[i][k];
                // Find all states r reachable from p on symbol j
                for (int l = 0; l < nfa_counts[p][j]; l++) {
                    int r = nfa_transitions[p][j][l];
                    addToSet(r, set_a, &count_a);
                }
            }

            // Now, find the union of ε-closures of all states in set_a
            count_b = 0; // This will be the final transition set
            for(int k = 0; k < count_a; k++) {
                int r = set_a[k];
                for (int l = 0; l < all_closures_counts[r]; l++) {
                    addToSet(all_closures[r][l], set_b, &count_b);
                }
            }

            // Store the resulting transition
            for (int k = 0; k < count_b; k++) {
                final_nfa_transitions[i][j][k] = set_b[k];
            }
            final_nfa_counts[i][j] = count_b;
        }
    }

    // 4. Calculate new final states
    // A state is final if its closure contains an original final state
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < all_closures_counts[i]; j++) {
            if (isInSet(all_closures[i][j], initial_final_states, num_final_states)) {
                addToSet(i, final_nfa_final_states, &final_nfa_final_count);
                break;
            }
        }
    }

    // 5. Print the resulting NFA without epsilon transitions
    printf("\n--- NFA without Epsilon Transitions ---\n");
    printf("Transition Table:\n");
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_symbols; j++) {
            printf("d'(q%d, %d) = { ", i, j);
            for (int k = 0; k < final_nfa_counts[i][j]; k++) {
                printf("q%d ", final_nfa_transitions[i][j][k]);
            }
            printf("}\n");
        }
    }

    printf("\nFinal States: { ");
    for (int i = 0; i < final_nfa_final_count; i++) {
        printf("q%d ", final_nfa_final_states[i]);
    }
    printf("}\n");

    return 0;
}

