%{
    #include "y.tab.h"
%}

%%

"for"   {return FOR;}
"("     {return LPAREN;}
")"     {return RPAREN;}
";"     {return SEMICOLON;}
"<"     {return LT;}
">"     {return GT;}
"<="    {return LE;}
">="    {return GE;}
"=="    {return EQ;}
"="     {return ASSIGN;}
"++"    {return INC;}
"--"    {return DEC;}

[0-9]+  {yylval = atoi(yytext); return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*  {return ID;}

[ \t\n]
.       {return yytext[0];}

%%

int yywrap(){
    return 1;
}