%{
    #include <stdio.h>
    #include <stdlib.h>

    void yyerror(const char* s);
    int yylex(void);
%}

%token WHILE LPAREN RPAREN EQ LE LT GE GT
%token NUMBER ID

%%

stmt : WHILE LPAREN expr_opt RPAREN
       {printf("Valid while loop");}
     ;
    
expr_opt :
         | expr
         ;

expr : ID
     | NUMBER
     | expr LT expr
     | expr GT expr
     | expr LE expr
     | expr GE expr
     | expr EQ expr
     ;

%%

void yyerror(const char* s){
    printf("Syntax error %s\n", s);
}

int main(){
    printf("Enter a while statement: ");
    yyparse();
    return 1;
}