%{
#include <stdio.h>
#include <ctype.h>

void yyerror(const char *s);
int yylex(void);
%}

%token LETTER DIGIT

%%

line:     variable '\n'   { 
                            printf("VALID variable\n\n"); 
                            printf("Enter a variable to check (or Ctrl+D to exit):\n");
                          }
        | error '\n'      { 
                            yyerrok;
                            printf("Enter a variable to check (or Ctrl+D to exit):\n");
                          }
        ;

variable: LETTER
        | variable LETTER
        | variable DIGIT
        ;

%%

int main() {
    printf("Enter a variable to check (or Ctrl+D to exit):\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "INVALID variable\n\n");
}

