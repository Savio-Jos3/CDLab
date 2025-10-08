%{
    #include "y.tab.h"
%}

%%

"if"    {return IF;}
"else"  {return ELSE;}
"("     {return LPAREN;}
")"     {return RPAREN;}
"<"     {return LT;}
"<="    {return LE;}
">"     {return GT;}
">="    {return GE;}
"=="    {return EQ;}
"&&"    {return AND;}
"||"    {return OR;}

[0-9]+  {yylval = atoi(yytext); return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*  {return ID;}

%%

int yywrap(){
    return 1;
}