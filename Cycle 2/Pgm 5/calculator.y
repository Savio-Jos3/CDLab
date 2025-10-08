%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Abstract Syntax Tree Node Structure --- */
typedef struct AST {
    char *nodeType; // E.g., "+", "*", "NUM"
    int value;      // Only used for "NUM" nodes
    struct AST *left, *right; // Children in the tree
} AST;

/* --- C Function Declarations --- */
AST* makeNode(char *type, AST *l, AST *r);
AST* makeNodeNum(int val);
void printAST(AST *root, int level);
int yylex(void);
int yyerror(char *s);

%}

/* --- YACC Declarations --- */

/* %union defines the possible types for grammar symbols. */
%union {
    int num;           // For the value of a NUMBER token
    struct AST *node;  // For the AST nodes built by the grammar rules
}

/* Associate the NUMBER token with the 'num' type from the union. */
%token <num> NUMBER
/* Associate the 'expr' non-terminal with the 'node' type. */
%type <node> expr

/* Define operator precedence and associativity to resolve ambiguity. */
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS /* A pseudo-token for unary minus precedence */

%%
/* --- Grammar Rules --- */

start : expr {
            printf("\n--- Abstract Syntax Tree ---\n");
            printAST($1, 0);
            printf("\n");
        }
      ;

expr  : expr '+' expr     { $$ = makeNode("+", $1, $3); }
      | expr '-' expr     { $$ = makeNode("-", $1, $3); }
      | expr '*' expr     { $$ = makeNode("*", $1, $3); }
      | expr '/' expr     { $$ = makeNode("/", $1, $3); }
      | '-' expr %prec UMINUS { $$ = makeNode("NEG", $2, NULL); }
      | '(' expr ')'      { $$ = $2; }
      | NUMBER            { $$ = makeNodeNum($1); }
      ;
%%

/* --- C Code Section --- */

AST* makeNode(char *type, AST *l, AST *r) {
    AST *node = (AST*) malloc(sizeof(AST));
    if (!node) {
        yyerror("out of memory");
        exit(0);
    }
    node->nodeType = strdup(type);
    node->value = 0;
    node->left = l;
    node->right = r;
    return node;
}

AST* makeNodeNum(int val) {
    AST *node = (AST*) malloc(sizeof(AST));
    if (!node) {
        yyerror("out of memory");
        exit(0);
    }
    node->nodeType = strdup("NUM");
    node->value = val;
    node->left = node->right = NULL;
    return node;
}

void printAST(AST *root, int level) {
    if (!root) return;
    for (int i=0; i<level; i++) printf("  ");
    
    if (strcmp(root->nodeType, "NUM") == 0) {
        printf("%s(%d)\n", root->nodeType, root->value);
    } else {
        printf("%s\n", root->nodeType);
    }
    
    printAST(root->left, level + 1);
    printAST(root->right, level + 1);
}

int main() {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}

int yyerror(char *s) {
    printf("Error: %s\n", s);
    return 0;
}
