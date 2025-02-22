%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

%}

%option yylineno

%%

"return"              { return RETURN; }
"void"                { return VOID; }
"User"                { return USER_TYPE; }
"bool"                { return BOOL_TYPE; }
"int"                 { return INT_TYPE; }
"Request"             { return REQUEST_TYPE; }
"UserData"            { return USERDATA_TYPE; }

"//".*               { /* ignore comments */ }

[ \t\r\n]+           { /* ignore whitespace */ }

\"([^\\\"]|\\.)*\"   { yylval.str = strdup(yytext); return STRING_LITERAL; }

[0-9]+               { yylval.intVal = atoi(yytext); return INT_LITERAL; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }

"("                  { return '('; }
")"                  { return ')'; }
"{"                  { return '{'; }
"}"                  { return '}'; }
","                  { return ','; }
";"                  { return ';'; }
"*"                  { return '*'; }
"."                  { return '.'; }

.                    { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}
