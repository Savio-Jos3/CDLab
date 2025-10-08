%{
    #include <stdio.h>
    #include <stdlib.h>

    void yyerror(const char* s);
    int yylex(void);
%}

%token IF ELSE LT GT LE GE EQ LPAREN RPAREN AND OR
%token ID NUMBER

%%

stmt : IF LPAREN expr_opt RPAREN
       {printf("Valid If-Else statement\n");}
     | ELSE IF LPAREN expr_opt RPAREN
       {printf("Valid If-Else statement\n");}
     | ELSE
       {printf("Valid If-Else statement\n");}
     ;

expr_opt : 
         | expr
         ;

expr : ID
     | NUMBER
     | expr LT expr
     | expr LE expr
     | expr GT expr
     | expr GE expr
     | expr EQ expr
     | expr OR expr
     | expr AND expr
     ;

%%

void yyerror(const char* s){
    printf("Invalid If-Else statement: %s", s);
}

int main(){
    printf("Enter if-else statement: ");
    yyparse();
    return 0;
}