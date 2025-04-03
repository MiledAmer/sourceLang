%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}

%%

"component"                     { return COMPONENT; }
"return"                        { return RETURN; }
"="                             { return EQUALS; }
"\""                            { return DOUBLE_QUOTE; }
"("                             { return LPAREN; }
")"                             { return RPAREN; }
"<"                             {return LT;}
">"                             {return GT;}
"/"                             {return SLASH;}
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
","                             { return COMMA; }
":"                             { return COLON; }
[a-zA-Z_][a-zA-Z0-9_]*          { yylval.strval = strdup(yytext); return IDENTIFIER; }
.                               { /* Unexpected character */ }
[ \t\n]+ { /* ignore whitespace */ }


%%

int yywrap() {
    return 1;
}
