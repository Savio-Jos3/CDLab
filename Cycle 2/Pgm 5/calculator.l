%{
#include "calculator.tab.h" // Include header from Bison
#include <stdlib.h>
// yylval is the union that holds the semantic value of the token
extern YYSTYPE yylval;
%}
%%
[0-9]+ { yylval.num = atoi(yytext); return NUMBER; }
[-+*/()] { return yytext[0]; }
[\n] { return 0; /* End of input for the parser */ }
[ \t] { /* ignore whitespace */ }
. { /* ignore other characters */ }
%%
int yywrap() {
return 1;
}