#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Data Structures ---
// Structure to hold a single Three-Address Code instruction
struct Instruction {
    char op[2];
    char arg1[5];
    char arg2[5];
    char res[5];
};

// --- Function Prototypes ---
void generate_assembly(struct Instruction tac[], int count);

// --- Backend: 8086 Assembly Code Generation ---
void generate_assembly(struct Instruction tac[], int count) {
    printf("\n\n--- 8086 Assembly Code ---\n\n");
    printf(".MODEL SMALL\n");
    printf(".STACK 100H\n");
    printf(".DATA\n");
    // In a real compiler, all variables (a, b, t0, t1, etc.)
    // would be declared here as DW (Define Word)
    printf("    ; Variables would be declared here, e.g.,\n");
    printf("    ; a DW ?\n");
    printf("    ; t0 DW ?\n");
    printf(".CODE\n");
    printf("MAIN PROC\n");
    printf("    MOV AX, @DATA\n");
    printf("    MOV DS, AX\n\n");

    for (int i = 0; i < count; i++) {
        printf("    ; TAC: %s = %s %s %s\n", tac[i].res, tac[i].arg1, tac[i].op, tac[i].arg2);
        
        // Determine the assembly instruction
        char instruction[4];
        if (strcmp(tac[i].op, "+") == 0) strcpy(instruction, "ADD");
        else if (strcmp(tac[i].op, "-") == 0) strcpy(instruction, "SUB");
        else if (strcmp(tac[i].op, "*") == 0) strcpy(instruction, "MUL");
        else if (strcmp(tac[i].op, "/") == 0) strcpy(instruction, "DIV");

        // Generate code
        printf("    MOV AX, %s\n", tac[i].arg1);
        
        if (strcmp(instruction, "MUL") == 0 || strcmp(instruction, "DIV") == 0) {
            // For MUL/DIV, the other operand is implicitly AX, so we move arg2 to another register
            printf("    MOV BX, %s\n", tac[i].arg2);
            printf("    %s BX\n", instruction);
        } else {
            // For ADD/SUB, the format is destination, source
            printf("    %s AX, %s\n", instruction, tac[i].arg2);
        }
        
        // Store the result from AX back into the result variable
        printf("    MOV %s, AX\n\n", tac[i].res);
    }

    printf("    ; Terminate the program\n");
    printf("    MOV AH, 4CH\n");
    printf("    INT 21H\n");
    printf("MAIN ENDP\n");
    printf("END MAIN\n");
}

// --- Main Driver ---
int main() {
    struct Instruction tac[30]; // Array to store the TAC
    int tac_index = 0;
    char line[100];

    printf("Enter Three-Address Code (e.g., t0 = a + b), one instruction per line.\n");
    printf("Type 'done' when you are finished.\n\n");

    while (tac_index < 30) {
        printf("TAC [%d]: ", tac_index);
        fgets(line, sizeof(line), stdin);

        // Check for termination condition
        if (strncmp(line, "done", 4) == 0) {
            break;
        }

        // Parse the line: res = arg1 op arg2
        // Example: t0 = a + b
        int items = sscanf(line, "%s = %s %1s %s", 
                           tac[tac_index].res, 
                           tac[tac_index].arg1, 
                           tac[tac_index].op, 
                           tac[tac_index].arg2);

        if (items == 4) {
            tac_index++;
        } else if (strlen(line) > 1) { // Ignore empty lines
            printf("Invalid format. Please use: result = arg1 op arg2\n");
        }
    }

    if (tac_index > 0) {
        // Generate the assembly code from the stored TAC
        generate_assembly(tac, tac_index);
    } else {
        printf("\nNo instructions entered.\n");
    }

    return 0;
}
