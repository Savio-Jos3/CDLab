%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%union {
    int ival;
}
%token <ival> NUMBER

%type <ival> expression
%left ADD SUB
%left MUL DIV

%%

line:     expression '\n'     { printf("= %d\n\n", $1); }
        | error '\n'          { yyerrok; }
        ;

expression: NUMBER              { $$ = $1; }
          | expression ADD expression { $$ = $1 + $3; }
          | expression SUB expression { $$ = $1 - $3; }
          | expression DIV expression {
                                        if ($3 == 0) {
                                            yyerror("Division by zero");
                                            $$ = 0;
                                        } else {
                                            $$ = $1 / $3;
                                        }
                                      }
          | LPAREN expression RPAREN  { $$ = $2; }
          ;

%%

int main() {
    printf("--- Simple Integer Calculator ---\nEnter an expression or Ctrl+D to exit.\n\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

