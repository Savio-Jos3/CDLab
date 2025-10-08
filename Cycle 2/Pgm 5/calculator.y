%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Define the structure for an Abstract Syntax Tree node
typedef struct AST {
char *nodeType;
int value;
struct AST *left,
*right;
} AST;
// Function prototypes
AST* makeNode(char *type, AST *l, AST *r);
AST* makeNodeNum(int val);
void printAST(AST *root, int level);
int yylex(void);
int yyerror(char *s);
// Helper function to create a new operator node
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
// Helper function to create a new number leaf node
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
// Function to print the AST in a tree-like format
void printAST(AST *root, int level) {
if (!root) return;
for (int i = 0; i < level; i++) printf(" ");
if (strcmp(root->nodeType,
"NUM") == 0)
printf("%s(%d)\n"
, root->nodeType, root->value);
else
printf("%s\n"
, root->nodeType);
printAST(root->left, level + 1);
printAST(root->right, level + 1);
}
%}
// Define the union for semantic values
%union {
int num;
struct AST *node;
}
// Associate tokens and non-terminals with union types
%token <num> NUMBER
%type <node> expr
// Define operator precedence and associativity
%left '+' '
'
-
%left '*' '/'
%nonassoc UMINUS
%%
// Grammar rules
start:
expr { printf("\nAbstract Syntax Tree:\n"); printAST($1, 0); }
;
expr:
expr '+' expr { $$ = makeNode("+"
, $1, $3); }
| expr '
-
' expr { $$ = makeNode("
"
-
, $1, $3); }
| expr '*' expr { $$ = makeNode("*"
, $1, $3); }
| expr '/' expr { $$ = makeNode("/"
, $1, $3); }
| '
-
' expr %prec UMINUS { $$ = makeNode("NEG"
, $2, NULL); }
| '(' expr ')' { $$ = $2; }
| NUMBER { $$ = makeNodeNum($1); }
;
%%
// Main program entry point
int main() {
printf("Enter an arithmetic expression:\n");
yyparse();
return 0;
}
// Custom error handling function
int yyerror(char *s) {
printf("Error: %s\n"
, s);
return 0;
}