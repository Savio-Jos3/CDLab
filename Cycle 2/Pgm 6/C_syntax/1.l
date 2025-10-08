%{
    #include "y.tab.h"    
%}

%%

"while"     {return WHILE;}
"("         {return LPAREN;}
")"         {return RPAREN;}
"<"         {return LT;}
">"         {return GT;}
"<="        {return LE;}
">="        {return GE;}
"=="        {return EQ;}

[0-9]+      {yylval = atoi(yytext); return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*  {return ID;}

%%

int yywrap(){
    return 1;
}