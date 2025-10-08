%{
    #include<stdio.h>
%}

%%
{
    .*"Savio".*\n   {printf("This string contains 'Savio':\t%s\n",yytext);}
    .*\n    {printf("%s\n",yytext);}
}
%%
int main(){
    printf("Enter");
    yylex();
    return 0;
}

int yywrap(){
    return 0;
}