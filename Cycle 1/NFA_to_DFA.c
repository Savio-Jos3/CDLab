#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 50
#define MAX_SYMBOLS 10

// --- Global Data Structures for NFA ---
int nfa_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int nfa_counts[MAX_STATES][MAX_SYMBOLS] = {0};
int nfa_final_states[MAX_STATES];
int nfa_num_states, nfa_num_symbols, nfa_num_final;

// --- Global Data Structures for DFA ---
// Each DFA state is a set of NFA states
int dfa_states[MAX_STATES][MAX_STATES];
int dfa_state_counts[MAX_STATES] = {0};
// The DFA transition table
int dfa_transitions[MAX_STATES][MAX_SYMBOLS];
int dfa_num_states = 0;

// --- Set Utility Functions ---
// Comparison function for qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

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

// --- Main Conversion Logic ---
int main() {
    // 1. Get NFA Input
    printf("--- NFA Input ---\n");
    printf("Enter the number of states: ");
    scanf("%d", &nfa_num_states);
    printf("Enter the number of symbols: ");
    scanf("%d", &nfa_num_symbols);

    printf("\nEnter transitions (from_state symbol to_state). Use -1 to end input for a given state/symbol.\n");
    for (int i = 0; i < nfa_num_states; i++) {
        for (int j = 0; j < nfa_num_symbols; j++) {
            printf("Transitions from state q%d on symbol %d: ", i, j);
            int to_state;
            while (1) {
                scanf("%d", &to_state);
                if (to_state == -1) break;
                nfa_transitions[i][j][nfa_counts[i][j]++] = to_state;
            }
        }
    }

    printf("\nEnter number of final states: ");
    scanf("%d", &nfa_num_final);
    printf("Enter the NFA final states: ");
    for (int i = 0; i < nfa_num_final; i++) {
        scanf("%d", &nfa_final_states[i]);
    }

    // --- 2. Subset Construction Algorithm ---
    int unprocessed_dfa_states[MAX_STATES]; // A queue of DFA state indices to process
    int queue_front = 0, queue_rear = 0;

    // Initialize: The first DFA state is the set containing the NFA start state (q0)
    dfa_states[0][0] = 0;
    dfa_state_counts[0] = 1;
    dfa_num_states = 1;
    unprocessed_dfa_states[queue_rear++] = 0; // Enqueue the first DFA state

    // Main loop: Process states until the queue is empty
    while (queue_front < queue_rear) {
        int current_dfa_idx = unprocessed_dfa_states[queue_front++]; // Dequeue

        // For each input symbol...
        for (int i = 0; i < nfa_num_symbols; i++) {
            int new_dfa_state[MAX_STATES];
            int new_dfa_count = 0;

            // Calculate the destination set of states from the current DFA state
            for (int j = 0; j < dfa_state_counts[current_dfa_idx]; j++) {
                int nfa_state = dfa_states[current_dfa_idx][j];
                for (int k = 0; k < nfa_counts[nfa_state][i]; k++) {
                    addToSet(nfa_transitions[nfa_state][i][k], new_dfa_state, &new_dfa_count);
                }
            }

            if (new_dfa_count == 0) {
                 dfa_transitions[current_dfa_idx][i] = -1; // No transition (trap state)
                 continue;
            }

            // Sort the new set to allow for easy comparison
            qsort(new_dfa_state, new_dfa_count, sizeof(int), compare);

            // Check if this new DFA state already exists
            int found_idx = -1;
            for (int k = 0; k < dfa_num_states; k++) {
                if (new_dfa_count == dfa_state_counts[k]) {
                    if (memcmp(new_dfa_state, dfa_states[k], new_dfa_count * sizeof(int)) == 0) {
                        found_idx = k;
                        break;
                    }
                }
            }

            if (found_idx != -1) { // State already exists
                dfa_transitions[current_dfa_idx][i] = found_idx;
            } else { // It's a new DFA state
                // Add it to our list of DFA states
                memcpy(dfa_states[dfa_num_states], new_dfa_state, new_dfa_count * sizeof(int));
                dfa_state_counts[dfa_num_states] = new_dfa_count;
                
                // Record the transition
                dfa_transitions[current_dfa_idx][i] = dfa_num_states;
                
                // Enqueue the new state to be processed later
                unprocessed_dfa_states[queue_rear++] = dfa_num_states;
                dfa_num_states++;
            }
        }
    }

    // --- 3. Determine DFA Final States and Print Results ---
    printf("\n--- Equivalent DFA ---\n");
    printf("Transition Table:\n");

    for (int i = 0; i < dfa_num_states; i++) {
        printf("D%d = { ", i);
        for (int j = 0; j < dfa_state_counts[i]; j++) {
            printf("q%d ", dfa_states[i][j]);
        }
        printf("}\n");

        for (int j = 0; j < nfa_num_symbols; j++) {
             if(dfa_transitions[i][j] != -1) {
                printf("\td(D%d, %d) = D%d\n", i, j, dfa_transitions[i][j]);
             } else {
                printf("\td(D%d, %d) = --\n", i, j);
             }
        }
        printf("\n");
    }

    printf("DFA Final States: { ");
    for (int i = 0; i < dfa_num_states; i++) {
        for (int j = 0; j < dfa_state_counts[i]; j++) {
            if (isInSet(dfa_states[i][j], nfa_final_states, nfa_num_final)) {
                printf("D%d ", i);
                break; // No need to check other states in this set
            }
        }
    }
    printf("}\n");

    return 0;
}
