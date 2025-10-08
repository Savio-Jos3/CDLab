#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 50

// --- Global variables for the NFA and calculations ---

// Adjacency list to store epsilon transitions.
// transitions[i] will store a list of states reachable from state 'i'.
int transitions[MAX_STATES][MAX_STATES];
// transitions_count[i] will store the number of transitions from state 'i'.
int transitions_count[MAX_STATES] = {0};

// Array to store the final closure set for a state.
int closure_set[MAX_STATES];
int closure_count = 0;

/**
 * @brief Checks if a state is already present in the closure set.
 * @param state The state to check.
 * @return 1 if the state is in the set, 0 otherwise.
 */
int isInClosure(int state) {
    for (int i = 0; i < closure_count; i++) {
        if (closure_set[i] == state) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Adds a state to the closure set if it's not already present.
 * @param state The state to add.
 */
void addToClosure(int state) {
    if (!isInClosure(state)) {
        closure_set[closure_count++] = state;
    }
}

/**
 * @brief Performs a Depth First Search (DFS) to find all reachable states
 * via epsilon transitions using a simple array-based stack.
 * @param start_state The state to start the search from.
 */
void findEpsilonClosure(int start_state) {
    int stack[MAX_STATES];
    int top = -1; // Stack pointer

    // Reset the closure set for the new calculation.
    closure_count = 0;

    // The closure of a state always includes the state itself.
    addToClosure(start_state);
    stack[++top] = start_state; // Push start_state onto the stack

    while (top != -1) {
        int u = stack[top--]; // Pop a state from the stack

        // Iterate over all states reachable from u via an ε-transition.
        for (int i = 0; i < transitions_count[u]; i++) {
            int v = transitions[u][i];
            // If this state is not in the closure set yet...
            if (!isInClosure(v)) {
                // ...add it and push it onto the stack to visit later.
                addToClosure(v);
                stack[++top] = v;
            }
        }
    }
}

int main() {
    int num_states, num_transitions;

    printf("Enter the number of states (max %d): ", MAX_STATES);
    scanf("%d", &num_states);

    printf("Enter the number of epsilon transitions: ");
    scanf("%d", &num_transitions);

    printf("Enter the epsilon transitions (from_state to_state):\n");
    for (int i = 0; i < num_transitions; ++i) {
        int from, to;
        scanf("%d %d", &from, &to);

        // Add the transition to our adjacency list.
        if (from < MAX_STATES && transitions_count[from] < MAX_STATES) {
            transitions[from][transitions_count[from]++] = to;
        }
    }

    printf("\n-------------------------------------\n");
    printf("Epsilon Closures for each state:\n");
    printf("-------------------------------------\n");

    // Calculate and print the ε-closure for every state in the NFA.
    for (int i = 0; i < num_states; ++i) {
        findEpsilonClosure(i);
        
        // Print the result.
        printf("e-closure(q%d) = { ", i);
        // Note: The set is not guaranteed to be sorted in this C version.
        for (int j = 0; j < closure_count; j++) {
            printf("q%d ", closure_set[j]);
        }
        printf("}\n");
    }

    return 0;
}

    
