%{
#include "variable_parser.tab.h" 
%}

%%
[a-zA-Z]    { return LETTER; }
[0-9]       { return DIGIT; }
\n          { return '\n'; } /* Pass newlines to the parser */
.           { /* Ignore any other character that is not a letter or digit */ }
%%

int yywrap() {
    return 1;
}

