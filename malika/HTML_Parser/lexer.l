%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>

%}

%%

"component"                     { return COMPONENT; }
"import"                        { return IMPORT; }
"from"                          { return FROM; }
"type"                          { return TYPE; }
"return"                        { return RETURN; }
"="                             { return EQUALS; }
"("                             { return LPAREN; }
")"                             { return RPAREN; }
"<"                             {return LT;}
">"                             {return GT;}
"/"                             {return SLASH;}
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
";"                             { return SEMICOLON; }
","                             { return COMMA; }
":"                             { return COLON; }
\"[^\"]*\"                      { yylval.strval = strdup(yytext); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]*          { yylval.strval = strdup(yytext); return IDENTIFIER; }
.                               { /* Unexpected character */ }
[ \t\n]+                          { /* ignore whitespace */ }


%%

int yywrap() {
    return 1;
}
