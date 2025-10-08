%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

int yyerror(char *s);
%}

%%
[0-9]+      {
                yylval.num = atoi(yytext);
                return NUMBER;
            }

[-+*/()\n]   { return *yytext; }

[ \t]       { }

.           { yyerror("Invalid character"); }

%%

int yywrap() {
    return 1;
}

