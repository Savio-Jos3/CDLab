%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex(void);
    void yyerror(const char* s);
%}

%token FOR LPAREN RPAREN SEMICOLON LT GT LE GE EQ ASSIGN INC DEC
%token NUMBER ID

%%

stmt : FOR LPAREN expr_opt SEMICOLON expr_opt SEMICOLON expr_opt RPAREN
       {printf("Valid for loop\n");}
     ;

expr_opt : 
         | expr
         ;

expr : ID ASSIGN NUMBER
     | ID
     | NUMBER
     | expr LT expr
     | expr GT expr
     | expr LE expr
     | expr GE expr
     | expr EQ expr
     | expr INC
     | expr DEC
     ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(){
    printf("Enter a for statement: ");
    yyparse();
    return 0;
}