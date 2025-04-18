%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>

%}

%%
"function"                      { return FUNCTION; }
"component"                     { return COMPONENT; }
"import"                        { return IMPORT; }
"from"                          { return FROM; }
"type"                          { return TYPE; }
"return"                        { return RETURN; }
"="                             { return EQUALS; }
"("                             { return LPAREN; }
"["                             {return LBRACKET; }
"]"                             {return RBRACKET; }
")"                             { return RPAREN; }
"<"                             {return LT;}
">"                             {return GT;}
"/"                             {return SLASH;}
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
";"                             { return SEMICOLON; }
","                             { return COMMA; }
":"                             { return COLON; }
"."                             { return DOT; }
\"[^\"]*\"                      { yylval.strval = strdup(yytext); return STRING_LITERAL; }
[0-9]+(\.[0-9]+)?               { yylval.strval = strdup(yytext); return NUMBER_LITERAL; }
true|false                      { yylval.strval = strdup(yytext); return BOOLEAN_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]*          { yylval.strval = strdup(yytext); return IDENTIFIER; }
.                               { /* Unexpected character */ }
[ \t\n]+                          { /* ignore whitespace */ }


%%

int yywrap() {
    return 1;
}
